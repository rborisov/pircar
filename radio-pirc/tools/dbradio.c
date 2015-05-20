#include <stdio.h>
#include "sqlitedb.h"

char *sqlchar0, *sqlchar1, *sqlchar2;

char *get_radio_url(char* radio)
{
    sqlite3_free(sqlchar0);
    sqlchar0 = sql_get_text_field(conn, "SELECT radio FROM Radios WHERE "
                    "radio = '%s' AND artist = '%s'", song, artist);
    return sqlchar0;
}

int db_add_radio(char *radio, char *url)
{

}

int main(int argc, char **argv)
{
    char *err_msg = 0;

    db_init();

    db_close();

    return 0;

}

