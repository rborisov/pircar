#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <syslog.h>

#include "mpd_client.h"
#include "mpd_utils.h"
#include "sqlitedb.h"
//#include "radio.h"

int mpd_crop(struct mpd_connection *conn)
{
    struct mpd_status *status = mpd_run_status(conn);
    if (status == 0)
        return 0;
    int length = mpd_status_get_queue_length(status) - 1;

    if (length < 0) {
        mpd_status_free(status);
        syslog(LOG_INFO, "%s: A playlist longer than 1 song in length is required to crop.\n", __func__);
    } else if (mpd_status_get_state(status) == MPD_STATE_PLAY ||
            mpd_status_get_state(status) == MPD_STATE_PAUSE) {
        if (!mpd_command_list_begin(conn, false)) {
            syslog(LOG_ERR, "%s: mpd_command_list_begin failed\n", __func__);
            return 0; //printErrorAndExit(conn);
        }

        for (; length >= 0; --length)
            if (length != mpd_status_get_song_pos(status))
                mpd_send_delete(conn, length);

        mpd_status_free(status);

        if (!mpd_command_list_end(conn) || !mpd_response_finish(conn)) {
            syslog(LOG_ERR, "%s: mpd_command_list_end || mpd_response_finish failed\n", __func__);
            return 0; //printErrorAndExit(conn);
        }

        return 0;
    } else {
        mpd_status_free(status);
        syslog(LOG_INFO, "%s: You need to be playing to crop the playlist\n", __func__);
        return 0;
    }
    return 1;
}

int mpd_list_artists(struct mpd_connection *conn)
{
    int num = 0;

    mpd_search_db_tags(conn, MPD_TAG_ARTIST);
    if (!mpd_search_commit(conn)) {
        syslog(LOG_DEBUG, "%s: search_commit error\n", __func__);
        return 0;
    }

    struct mpd_pair *pair;
    while ((pair = mpd_recv_pair_tag(conn, MPD_TAG_ARTIST)) != NULL) {
        syslog(LOG_DEBUG, "%s: %s\n", __func__, pair->value);
        mpd_return_pair(conn, pair);
        num++;
    }

    if (!mpd_response_finish(conn)) {
        syslog(LOG_DEBUG, "%s: error\n", __func__);
        return 0;
    }

    syslog(LOG_DEBUG, "%s: found %i\n", __func__, num);
    return num;
}

char* mpd_get_artist(struct mpd_song const *song)
{
    char *str;
    str = (char *)mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    if(str == NULL)
        str = basename((char *)mpd_song_get_uri(song));
//printf("%i %s\n", &str, str);
    return str;
}

char* mpd_get_title(struct mpd_song const *song)
{
    char *str;

    str = (char *)mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    if(str == NULL){
        str = basename((char *)mpd_song_get_uri(song));
    }
//	printf("%i %s\n", &str, str);
    return str;
}

unsigned mpd_get_queue_length(struct mpd_connection *conn)
{
    struct mpd_status *status = mpd_run_status(conn);
    if (status == NULL)
        return 0;
    const unsigned length = mpd_status_get_queue_length(status);
    mpd_status_free(status);
    return length;
}

int mpd_insert ( struct mpd_connection *conn, char *song_path )
{
    struct mpd_status *status = mpd_run_status(conn);
    if (status == NULL)
        return 0;
    const unsigned from = mpd_status_get_queue_length(status);
    const int cur_pos = mpd_status_get_song_pos(status);
    mpd_status_free(status);

    if (mpd_run_add(conn, song_path) != true)
        return 0;

    /* check the new queue length to find out how many songs were
     *        appended  */
    const unsigned end = mpd_get_queue_length(conn);
    if (end == from)
        return 0;

    /* move those songs to right after the current one */
    return mpd_run_move_range(conn, from, end, cur_pos + 1);
}

void get_random_song(struct mpd_connection *conn, char *str, char *path)
{
    struct mpd_entity *entity;
    int listened0 = 65000,
        skipnum, numberofsongs = 0;

    struct mpd_stats *stats = mpd_run_stats(conn);
    if (stats == NULL)
        return;
    numberofsongs = mpd_stats_get_number_of_songs(stats);
    mpd_stats_free(stats);
    skipnum = rand() % numberofsongs;

    syslog(LOG_DEBUG, "%s: path %s; number of songs: %i skip: %i\n",
            __func__, path, numberofsongs, skipnum);
    if (!mpd_send_list_all_meta(conn, ""))//path))
    {
        syslog(LOG_ERR, "%s: error: mpd_send_list_meta %s\n", __func__, path);
        return;
    }

//    srand((unsigned)time(0));

    while((entity = mpd_recv_entity(conn)) != NULL)
    {
        const struct mpd_song *song;
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG)
        {
            if (skipnum-- > 0)
                continue;

            int listened;
            song = mpd_entity_get_song(entity);
            listened = db_get_song_numplayed(mpd_get_title(song),
                    mpd_get_artist(song));
            if (listened < listened0) {
                listened0 = listened;
                syslog(LOG_DEBUG, "listened: %i ", listened);
                int probability = 50 +
                    db_get_song_rating(mpd_get_title(song),
                            mpd_get_artist(song));
                syslog(LOG_DEBUG, "probability: %i ", probability);
                bool Yes = (rand() % 100) < probability;
                if (Yes) {
                    sprintf(str, "%s", mpd_song_get_uri(song));
                    syslog(LOG_DEBUG, "uri: %s ", str);
                    syslog(LOG_DEBUG, "title: %s ", mpd_get_title(song));
                    syslog(LOG_DEBUG, "artist: %s", mpd_get_artist(song));
                }
            }
        }
        mpd_entity_free(entity);
    }
}

void get_song_to_delete(char *str)
{
    struct mpd_entity *entity;
    int rating0 = 65000;
    if (!mpd_send_list_meta(mpd.conn, "/")) {
        syslog(LOG_DEBUG, "error: mpd_send_list_meta\n");
        return;
    }
    while((entity = mpd_recv_entity(mpd.conn)) != NULL) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
            const struct mpd_song *song = mpd_entity_get_song(entity);
            int rating = db_get_song_rating(mpd_get_title(song),
                    mpd_get_artist(song));
            if (rating < rating0) {
                rating0 = rating;
                sprintf(str, "%s", mpd_song_get_uri(song));
            }
        }
        mpd_entity_free(entity);
    }
}

int get_current_song_rating()
{
    int rating;
    struct mpd_song *song;

    song = mpd_run_current_song(mpd.conn);
    if(song == NULL)
        return 0;

    rating = db_get_song_rating(mpd_get_title(song), mpd_get_artist(song));

    mpd_song_free(song);
    return rating;
}

int mpd_delete_current_song(struct mpd_connection *conn)
{
    struct mpd_song *song;
    char *currentsonguri = NULL;

    song = mpd_run_current_song(conn);
    if(song == NULL)
        return 0;

    db_update_song_rating(mpd_get_title(song),
            mpd_song_get_tag(song, MPD_TAG_ARTIST, 0), -5);

    currentsonguri = mpd_song_get_uri(song);
    printf("%s: let's delete song: %s\n", __func__, currentsonguri);

    delete_file_forever(currentsonguri);

    mpd_song_free(song);

    return 1;
}
