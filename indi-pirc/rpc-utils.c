#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpd/client.h>
#include "rpc-utils.h"

#define OPT_HOST NULL
#define OPT_PORT NULL

char buffer[64] = "";
struct mpd_connection *conn;

char* rpc_song_string()
{
//    printf("-");
    if (!mpd_command_list_begin(conn, true) ||
            !mpd_send_status(conn) ||
            !mpd_send_current_song(conn) ||
            !mpd_command_list_end(conn))
        goto end;
    
    struct mpd_status *status = mpd_recv_status(conn);
    if (status == NULL)
        printErrorAndExit(conn);
    
    if (mpd_status_get_state(status) == MPD_STATE_PLAY ||
            mpd_status_get_state(status) == MPD_STATE_PAUSE) {
        if (!mpd_response_next(conn))
            goto free; //printErrorAndExit(conn);

        struct mpd_song *song = mpd_recv_song(conn);
        if (song != NULL) {
            char *value;
            size_t size;
            memset(buffer,0,strlen(buffer));
            value = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
            size = strlen(value)<64?strlen(value):64;
            memcpy(buffer, value, size);
//            printf("artist: %s %s %i\n", value, buffer, size);
            mpd_song_free(song);
        }
    }
free: 
    mpd_status_free(status);
    if (!mpd_response_finish(conn))
        printErrorAndExit(conn);

end:
    return buffer;
}

struct mpd_status * getStatus(struct mpd_connection *conn)
{
    struct mpd_status *ret = mpd_run_status(conn);
    if (ret == NULL)
        printErrorAndExit(conn);

    return ret;
}

void printErrorAndExit(struct mpd_connection *conn)
{
    assert(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS);

    const char *message = mpd_connection_get_error_message(conn);
    fprintf(stderr, "mpd error: %s\n", message);
    mpd_connection_free(conn);
    exit(EXIT_FAILURE);
}

static struct mpd_connection * setup_connection(void)
{
    struct mpd_connection *conn = mpd_connection_new(OPT_HOST, OPT_PORT, 0);
    if (conn == NULL) {
        fputs("Out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
        printErrorAndExit(conn);

    return conn;
}

void finish(struct mpd_connection *conn)
{
    if (!mpd_response_finish(conn))
        printErrorAndExit(conn);
}

void cmd_pause(struct mpd_connection *conn)
{
    mpd_send_pause(conn, true);
    finish(conn);
}

void cmd_play(struct mpd_connection *conn)
{
    bool success;
    success = mpd_run_play(conn);

    if (!success)
        printErrorAndExit(conn);
}

void rpc_play_toggle()
{
   // struct mpd_connection *conn = setup_connection();
    struct mpd_status *status = getStatus(conn);
    if (mpd_status_get_state(status) == MPD_STATE_PLAY) {
        cmd_pause(conn);
    } else {
        cmd_play(conn);
    }
   // mpd_connection_free(conn);
}

void rpc_init()
{
    conn = setup_connection();
}

void rpc_free()
{
    mpd_connection_free(conn);
}
