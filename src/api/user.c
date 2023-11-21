#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "user.h"
#include "../lib/headers/database.h"
#include "../lib/headers/user.h"

void user_api_get(FILE *file) {
	const int length = atoi(getenv("CONTENT_LENGTH")) + 1;

	char *login_type = malloc(sizeof(char) * length);
	char *username = malloc(sizeof(char) * length);
	char *secret = malloc(sizeof(char) * length);
	
	fscanf(file, "login_type=%255[^&]&username=%255[^&]&secret=%255s", login_type, username, secret);

	user_t user;

	if (strcmp(login_type, "password") == 0) {
		user = user_select_with_password(username, secret);
	} else {
		user = user_select_with_answer(username, secret);
	}

	if (user.username) {
		printf("Content-Type: application/json\n\n");
		printf(
			"{"
			"\"username\": \"%s\","
			"\"token\": \"%s\""
			"}\n"
		, user.username, user.token);
	} else {
		printf("Status: 404\n\n");
	}

	free(login_type);
	free(username);
	free(secret);
	user_free(user);
}

void user_api_post(FILE *file) {
	const int length = atoi(getenv("CONTENT_LENGTH")) + 1;

	char *username = malloc(sizeof(char) * length);
	char *password = malloc(sizeof(char) * length);
	char *question = malloc(sizeof(char) * length);
	char *answer = malloc(sizeof(char) * length);
	
	fscanf(file, "username=%255[^&]&password=%255[^&]&question=%255[^&]&answer=%255s", username, password, question, answer);
	
	user_t existing_user = user_select_with_username(username);
	if (existing_user.username != NULL) {
		free(existing_user.username);
		free(existing_user.password);
		free(existing_user.answer);
		free(existing_user.token);
		printf("Status: 409\n\n");
	} else {
		user_insert(username, password, question, answer);
		printf("Status: 201\n\n");
	}

	free(username);
	free(password);
	free(question);
	free(answer);
}

void user_api_put(FILE *file) {
	const int length = atoi(getenv("CONTENT_LENGTH")) + 1;

	char *username = malloc(sizeof(char) * length);
	char *token = malloc(sizeof(char) * length);
	char *new_password = malloc(sizeof(char) * length);

	fscanf(file, "username=%255[^&]&token=%255[^&]&new_password=%255s", username, token, new_password);

	user_t existing_user = user_select_with_token(username, token);
	if (existing_user.username != NULL) {
		printf("Content-Type: application/json\n\n");
		printf("{"
			"\"username\": \"%s\","
			"\"token\": \"%s\""
			"}", existing_user.username, existing_user.token);

		user_update_password(username, new_password);
		free(existing_user.username);
		free(existing_user.password);
		free(existing_user.answer);
		free(existing_user.token);
	} else {
		printf("Status: 401\n\n");
	}

	free(username);
	free(token);
	free(new_password);
}
