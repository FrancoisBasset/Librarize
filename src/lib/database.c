#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include "headers/database.h"

char* database_get_path(void);

void database_init(void) {
	database_exec(
		"CREATE TABLE IF NOT EXISTS users"
		"('id' INTEGER NOT NULL, 'username' TEXT NOT NULL, 'password' TEXT NOT NULL, 'question' TEXT NOT NULL, 'answer' TEXT NOT NULL, 'token' TEXT,"
		"PRIMARY KEY('id' AUTOINCREMENT))");

	char *path = database_get_path();
	chmod(path, 0777);
	free(path);
}

char* database_get_path(void) {
	char *path = malloc(sizeof(char) * PATH_MAX);
	getcwd(path, PATH_MAX);

	if (strstr(path, "/api")) {
		path[strlen(path) - 4] = '\0';
	}

	strcat(path, "/librarize.db");

	return path;
}

int database_exec(const char *sql) {
	sqlite3 *db;
	char *path = database_get_path();
	sqlite3_open(path, &db);
	free(path);

	sqlite3_stmt *stmt;
	const char **unused_sql = NULL;

	int prepare_status = sqlite3_prepare_v2(db, sql, (int) strlen(sql) + 1, &stmt, unused_sql);
	
	if (prepare_status != SQLITE_OK) {
		sqlite3_finalize(stmt);
		return -1;
	}

	int step_status = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_close(db);

	if (step_status != SQLITE_DONE) {
		return -1;
	}

	return 0;
}

sqlite3_value*** database_select(const char *sql, int *length) {
	sqlite3 *db;
	char *path = database_get_path();
	sqlite3_open(path, &db);
	free(path);

	sqlite3_stmt *stmt;
	const char **tail = NULL;

	int prepare_status = sqlite3_prepare_v2(db, sql, (int) strlen(sql), &stmt, tail);

	if (prepare_status != SQLITE_OK) {
		sqlite3_finalize(stmt);
		return NULL;
	}

	const int column_count = sqlite3_column_count(stmt);

	sqlite3_value ***results = malloc(sizeof(sqlite3_value**));
	int stmt_status = sqlite3_step(stmt);
	int i = 0;

	while (stmt_status == 100) {
		results = realloc(results, (i + 1) * sizeof(sqlite3_value**));

		results[i] = malloc(sizeof(sqlite3_value*) * column_count);
		for (int j = 0; j < column_count; j++) {
			results[i][j] = sqlite3_value_dup(sqlite3_column_value(stmt, j));
		}

		stmt_status = sqlite3_step(stmt);

		(*length)++;
		i++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return results;
}
