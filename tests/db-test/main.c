#include <stdio.h>
#include <sqlite3.h>

sqlite3 *db;
sqlite3_stmt *res;
char sql[80];

int sql_get_int_field(sqlite3 *db, const char *fmt, ...)
{
    va_list     ap;
    int     counter, result;
    char        *sql;
    int     ret;
    sqlite3_stmt    *stmt;

    va_start(ap, fmt);
    sql = sqlite3_vmprintf(fmt, ap);
    va_end(ap);
#if 0
    syslog(LOG_DEBUG, "%s sql: %s\n", __func__, sql);
#endif
    switch (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL))
    {
        case SQLITE_OK:
            break;
        default:
            printf("%s prepare failed: %s\n%s\n", __func__, sqlite3_errmsg(db), sql);
            sqlite3_free(sql);
            return -1;
    }

    for (counter = 0;
            ((result = sqlite3_step(stmt)) == SQLITE_BUSY || result == SQLITE_LOCKED) && counter < 2;
            counter++) {
        /* While SQLITE_BUSY has a built in timeout,
         *             SQLITE_LOCKED does not, so sleep */
        if (result == SQLITE_LOCKED)
            sleep(1);
    }
    switch (result)
    {
        case SQLITE_DONE:
            /* no rows returned */
            ret = 0;
            break;
        case SQLITE_ROW:
            if (sqlite3_column_type(stmt, 0) == SQLITE_NULL)
            {
                ret = 0;
                break;
            }
            ret = sqlite3_column_int(stmt, 0);
            break;
        default:
            printf("%s: step failed: %s\n%s\n", __func__, sqlite3_errmsg(db), sql);
            ret = -1;
            break;
    }
    sqlite3_free(sql);
    sqlite3_finalize(stmt);

    return ret;
}

void convert_str(char *instr)
{
    char *out = instr;

    if (!instr)
        return;

    while (*out) {
        if (*out == '\'')
            *out = '\"';
        out++;
    }
    *out = '\0';
}

int db_get_song_rating(char* song, char* artist)
{
    convert_str(song);
    convert_str(artist);
    int rating = sql_get_int_field(db, "SELECT rating FROM Songs WHERE "
            "song = '%s' AND artist = '%s'", song, artist);
    return rating;
}

//TODO: is not thread safe
void db_get_first(char *table, char *row, char *result)
{
    int rc;
    sprintf(sql, "SELECT %s FROM %s ORDER by lower(%s)", row, table, row);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            sprintf(result, "%s", sqlite3_column_text(res, 0));
            return;
        }
    }
    result[0] = '\0';
    sqlite3_finalize(res);
}

void db_get_next(char *result)
{
    if (sqlite3_step(res) == SQLITE_ROW) {
        sprintf(result, "%s", sqlite3_column_text(res, 0));
        return;
    }
    result[0] = '\0';
    sqlite3_finalize(res);
}

int db_get_prior_song_rating_first(char *result)
{
    int rc;
    rc = sqlite3_prepare_v2(db, "SELECT song, rating FROM Songs ORDER by numplayed", -1, &res, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            sprintf(result, "%s", sqlite3_column_text(res, 0));
            return sqlite3_column_int(res, 1);
        }
    }
    result[0] = '\0';
    sqlite3_finalize(res);

    return 0;
}

int db_get_song_rating_next(char *result)
{
    if (sqlite3_step(res) == SQLITE_ROW) {
        sprintf(result, "%s", sqlite3_column_text(res, 0));
        return sqlite3_column_int(res, 1);
    }
    result[0] = '\0';
    sqlite3_finalize(res);

    return 0;
}

int main(int argc, char **argv)
{
    char *err_msg = 0;
    char songname[180] = "";
    int rat = 0;

    int rc = sqlite3_open("/media/500gb/rcardb.sqlite", &db);

    if (rc != SQLITE_OK) {

        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }
#if 0
    db_get_first("Artists", "artist", &songname);
    while (songname[0]) {
        printf("%s\n", songname);
        db_get_next(&songname);
    }
#endif

    rat = db_get_prior_song_rating_first(&songname); 
    while (songname[0]) {
        printf("%s - %i\n", songname, rat);
        rat = db_get_song_rating_next(&songname);
    }

#if 0
    char *sql = "SELECT song FROM Songs";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {

 //       sqlite3_bind_int(res, 1, 3);
    } else {

        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step;
   
    do {
        step = sqlite3_step(res);

        if (step == SQLITE_ROW) {

            printf("%s: \n", sqlite3_column_text(res, 0));
//            printf("%s - ", sqlite3_column_text(res, 1));
//            printf("%s\n", sqlite3_column_text(res, 3));

        } 
    } while (step == SQLITE_ROW);

    sqlite3_finalize(res);

#endif
    sqlite3_close(db);

    return 0;

}

