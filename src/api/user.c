#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "user.h"
#include "../lib/headers/database.h"
#include "../lib/headers/user.h"

void user_api_get(void) {
	const int length = atoi(getenv("CONTENT_LENGTH")) + 1;

	char *login_type = malloc(sizeof(char) * length);
	char *username = malloc(sizeof(char) * length);
	char *secret = malloc(sizeof(char) * length);
	
	fscanf(stdin, "login_type=%255[^&]&username=%255[^&]&secret=%255s", login_type, username, secret);

	user_t user;

	if (strcmp(login_type, "password") == 0) {
		user = user_select_with_password(username, secret);
	} else {
		user = user_select_with_answer(username, secret);
	}

	if (user.username) {
		printf("Content-Type: appliction/json\n\n");
		printf(
			"{"
			"\"username\": \"%s\","
			"\"token\": \"%s\""
			"}\n"
		, user.username, user.token);
	} else {
		printf("Status: 404\n\n");
	}
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
