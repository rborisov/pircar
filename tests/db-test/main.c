#include <stdio.h>
#include <sqlite3.h>

sqlite3 *db;
sqlite3_stmt *res;
char sql[80];

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

int main(int argc, char **argv)
{
    char *err_msg = 0;
    char songname[180] = "";

    int rc = sqlite3_open("/media/storage/rcardb.sqlite", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    db_get_first("Artists", "artist", &songname);
    while (songname[0]) {
        printf("%s\n", songname);
        db_get_next(&songname);
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

