#ifndef DATABASE_H
#include <sqlite3.h>
void database_init(void);
int database_exec(const char *sql);
sqlite3_value*** database_select(const char *sql, int *length);
#endif