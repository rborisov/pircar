#ifndef __MPD_UTILS_H__
#define __MPD_UTILS_H__

#include <mpd/client.h>

unsigned mpd_get_queue_length(struct mpd_connection *);
int mpd_insert (struct mpd_connection *, char * );
int mpd_crop(struct mpd_connection *);
void get_random_song(struct mpd_connection *, char *, char *);
int get_current_song_rating();
char* mpd_get_title(struct mpd_song const *song);
int mpd_list_artists(struct mpd_connection *conn);
char* mpd_get_artist(struct mpd_song const *song);
void get_song_to_delete(char *str);

int mpd_delete_current_song(struct mpd_connection *conn);

#endif
