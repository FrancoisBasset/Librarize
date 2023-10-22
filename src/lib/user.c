#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/user.h"
#include "headers/database.h"

user_t user_select_from_sql(char *sql) {
	int length = 0;
	sqlite3_value ***users = database_select(sql, &length);

	free(sql);

	sqlite3_value *username_value = NULL;
	user_t user = (user_t) {};

	if (length != 0) {
		username_value = users[0][0];
		user.username = strdup((const char*) sqlite3_value_text(username_value));
		sqlite3_value_free(users[0][0]);
		sqlite3_free(users[0]);
	}

	free(users);

	return user;
}

user_t user_select_with_password(const char *username, const char *password) {
	char *sql = malloc(sizeof(char) * (66 + strlen(username) + strlen(password)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s' AND password = '%s'", username, password);

	return user_select_from_sql(sql);
}

user_t user_select_with_answer(const char *username, const char *answer) {
	char *sql = malloc(sizeof(char) * (64 + strlen(username) + strlen(answer)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s' AND answer = '%s'", username, answer);

	return user_select_from_sql(sql);
}

void user_insert(const char *username, const char *password, const char *question, const char *answer) {
	char *sql = malloc(sizeof(char) * (81 + strlen(username) + strlen(password) + strlen(question) + strlen(answer)));
	sprintf(
		sql,
		"INSERT INTO users (username, password, question, answer) VALUES ('%s', '%s', '%s', '%s')",
		username, password, question, answer);

	database_exec(sql);

	free(sql);
}
