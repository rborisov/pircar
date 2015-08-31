#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>

#include <mpd/client.h>

#define DIE(...) do { fprintf(stderr, __VA_ARGS__); return -1; } while(0)

enum {
    SEARCH_TAG_ANY = MPD_TAG_COUNT + 1,
    SEARCH_TAG_URI = MPD_TAG_COUNT + 2,
#if LIBMPDCLIENT_CHECK_VERSION(2,9,0)
    SEARCH_TAG_BASE,
#endif
};

struct constraint {
    enum mpd_tag_type type;
    char *query;
};

void printErrorAndExit(struct mpd_connection *conn)
{
    assert(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS);

    const char *message = mpd_connection_get_error_message(conn);
#if 0
    if (mpd_connection_get_error(conn) == MPD_ERROR_SERVER)
        /* messages received from the server are UTF-8; the
           rest is either US-ASCII or locale */
        message = charset_from_utf8(message);
#endif
    fprintf(stderr, "mpd error: %s\n", message);
    mpd_connection_free(conn);
    exit(EXIT_FAILURE);
}

void print_entity_list(struct mpd_connection *c, enum mpd_entity_type filter_type)
{
    struct mpd_entity *entity;
    while ((entity = mpd_recv_entity(c)) != NULL) {
        const struct mpd_directory *dir;
        const struct mpd_song *song;
        const struct mpd_playlist *playlist;

        enum mpd_entity_type type = mpd_entity_get_type(entity);
        if (filter_type != MPD_ENTITY_TYPE_UNKNOWN &&
            type != filter_type)
            type = MPD_ENTITY_TYPE_UNKNOWN;

        switch (type) {
        case MPD_ENTITY_TYPE_UNKNOWN:
            break;

        case MPD_ENTITY_TYPE_DIRECTORY:
            dir = mpd_entity_get_directory(entity);
            printf("%s\n", mpd_directory_get_path(dir)); //charset_from_utf8(mpd_directory_get_path(dir)));
            break;

        case MPD_ENTITY_TYPE_SONG:
            song = mpd_entity_get_song(entity);
            /*if (options.custom_format) {
                pretty_print_song(song);
                puts("");
            } else*/
                printf("%s\n", mpd_song_get_uri(song)); //charset_from_utf8(mpd_song_get_uri(song)));
            break;

        case MPD_ENTITY_TYPE_PLAYLIST:
            playlist = mpd_entity_get_playlist(entity);
            printf("%s\n", mpd_playlist_get_path(playlist)); //charset_from_utf8(mpd_playlist_get_path(playlist)));
            break;
        }

        mpd_entity_free(entity);
    }
}

void my_finishCommand(struct mpd_connection *conn)
{
    if (!mpd_response_finish(conn))
        printErrorAndExit(conn);
}

static struct mpd_connection *setup_connection(void)
{
    struct mpd_connection *conn = mpd_connection_new(NULL, NULL, 0);
    if (conn == NULL) {
        fputs("Out of memory\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
        printErrorAndExit(conn);

    return conn;
}

enum mpd_tag_type get_search_type(const char *name)
{
    if (strcasecmp(name, "any") == 0)
        return SEARCH_TAG_ANY;

    if (strcasecmp(name, "filename") == 0)
        return SEARCH_TAG_URI;

#if LIBMPDCLIENT_CHECK_VERSION(2,9,0)
    if (strcasecmp(name, "base") == 0)
        return SEARCH_TAG_BASE;
#endif

    enum mpd_tag_type type = mpd_tag_name_iparse(name);
    if (type != MPD_TAG_UNKNOWN)
        return type;

    fprintf(stderr, "\"%s\" is not a valid search type: <any", name);

    for (unsigned i = 0; i < MPD_TAG_COUNT; i++) {
        name = mpd_tag_name(i);
        if (name == NULL)
            continue;

        fputc('|', stderr);
        fputs(name, stderr);
    }

    fputs(">\n", stderr);

    return MPD_TAG_UNKNOWN;
}

int get_constraints(int argc, char **argv, struct constraint **constraints)
{
    int numconstraints = 0;
    int type;
    int i;

    assert(argc > 0 && argc % 2 == 0);

    *constraints = malloc(sizeof(**constraints) * argc / 2);

    for (i = 0; i < argc; i += 2) {
        type = get_search_type(argv[i]);
        if (type < 0) {
            free(*constraints);
            return -1;
        }

        (*constraints)[numconstraints].type = type;
        (*constraints)[numconstraints].query = argv[i+1];
        numconstraints++;
    }

    return numconstraints;
}

static void add_constraint(struct mpd_connection *conn,
           const struct constraint *constraint)
{
    if (constraint->type == (enum mpd_tag_type)SEARCH_TAG_ANY)
        mpd_search_add_any_tag_constraint(conn, MPD_OPERATOR_DEFAULT,
                                          constraint->query);
                          //charset_to_utf8(constraint->query));
    else if (constraint->type == (enum mpd_tag_type)SEARCH_TAG_URI)
        mpd_search_add_uri_constraint(conn, MPD_OPERATOR_DEFAULT,
                                      constraint->query);
                          //charset_to_utf8(constraint->query));
#if LIBMPDCLIENT_CHECK_VERSION(2,9,0)
    else if (constraint->type == (enum mpd_tag_type)SEARCH_TAG_BASE)
        mpd_search_add_base_constraint(conn, MPD_OPERATOR_DEFAULT,
                                       constraint->query);
                           //charset_to_utf8(constraint->query));
#endif
    else
        mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT,
                          constraint->type,
                                      constraint->query);
                          //charset_to_utf8(constraint->query));
}

bool add_constraints(int argc, char ** argv, struct mpd_connection *conn)
{
    struct constraint *constraints;

    if (argc % 2 != 0)
        DIE("arguments must be pairs of search types and queries\n");

    int numconstraints = get_constraints(argc, argv, &constraints);
    if (numconstraints < 0)
        return false;

    for (int i = 0; i < numconstraints; i++) {
        add_constraint(conn, &constraints[i]);
    }

    free(constraints);
    return true;
}

static int do_search(int argc, char ** argv, struct mpd_connection *conn, bool exact)
{
    mpd_search_db_songs(conn, exact);
    if (!add_constraints(argc, argv, conn))
        return -1;

    if (!mpd_search_commit(conn))
        printErrorAndExit(conn);

    print_entity_list(conn, MPD_ENTITY_TYPE_SONG);

    my_finishCommand(conn);

    return 0;
}

static int find_songname_id(struct mpd_connection *conn, const char *s)
{
    int res = -1;

    mpd_search_queue_songs(conn, false);

    const char *pattern = s; //charset_to_utf8(s);
    mpd_search_add_any_tag_constraint(conn, MPD_OPERATOR_DEFAULT,
                           pattern);
    mpd_search_commit(conn);

    struct mpd_song *song = mpd_recv_song(conn);
    if (song != NULL) {
        res = mpd_song_get_id(song);

        mpd_song_free(song);
    }

    my_finishCommand(conn);

    return res;
}

int main(int argc, char ** argv)
{
    struct mpd_connection *conn = setup_connection();
    int id = -1;
    char *args[] = {"artist", "Eels", "title", "Bus Stop Boxer"};
/*    if (!mpd_send_list_all_meta(conn, "radio"))
        printErrorAndExit(conn);

    print_entity_list(conn, MPD_ENTITY_TYPE_SONG);//MPD_ENTITY_TYPE_UNKNOWN);
*/
/*    id = find_songname_id(conn, "artist Eels");
    printf("id = %i\n", id);
*/
    do_search(4, args, conn, 1);
    mpd_connection_free(conn);

    return 0;
}
