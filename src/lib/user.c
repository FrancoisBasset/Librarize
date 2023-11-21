#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/user.h"
#include "headers/database.h"
#include "headers/utils.h"

void user_set_token(const char *username, const char *token) {
	char *sql = malloc(sizeof(char) * (50 + strlen(username) + strlen(token)));
	sprintf(sql, "UPDATE users SET token = '%s' WHERE username = '%s'", token, username);

	database_exec(sql);
	free(sql);
}

user_t user_select_from_sql(char *sql) {
	int length = 0;
	sqlite3_value ***users = database_select(sql, &length);

	free(sql);

	sqlite3_value *username_value = NULL;
	user_t user = (user_t) {};

	if (length != 0) {
		char *uuid = get_uuid();

		username_value = users[0][0];
		user.username = strdup((const char*) sqlite3_value_text(username_value));
		user.token = uuid;

		user_set_token(user.username, user.token);

		sqlite3_value_free(users[0][0]);
		sqlite3_free(users[0]);
	}

	free(users);

	return user;
}

user_t user_select_with_username(const char *username) {
	char *sql = malloc(sizeof(char) * (47 + strlen(username)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s'", username);

	return user_select_from_sql(sql);
}

user_t user_select_with_password(const char *username, const char *password) {
	char *sql = malloc(sizeof(char) * (65 + strlen(username) + strlen(password)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s' AND password = '%s'", username, password);

	return user_select_from_sql(sql);
}

user_t user_select_with_answer(const char *username, const char *answer) {
	char *sql = malloc(sizeof(char) * (63 + strlen(username) + strlen(answer)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s' AND answer = '%s'", username, answer);

	return user_select_from_sql(sql);
}

user_t user_select_with_token(const char *username, const char *token) {
	char *sql = malloc(sizeof(char) * (62 + strlen(username) + strlen(token)));
	sprintf(sql, "SELECT username FROM users WHERE username = '%s' AND token = '%s'", username, token);

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

void user_update_password(const char *username, const char *new_password) {
	char *sql = malloc(sizeof(char) * (51 + strlen(new_password) + strlen(username)));
	sprintf(sql, "UPDATE users SET password = '%s' WHERE username = '%s'", new_password, username);

	database_exec(sql);
	free(sql);
}

void user_free(user_t user) {
	free(user.username);
	free(user.password);
	free(user.question);
	free(user.answer);
	free(user.token);
}
