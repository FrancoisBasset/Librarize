#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/headers/database.h"
#include "lib/headers/fixtures.h"
#include "lib/headers/user.h"
#include "lib/headers/utils.h"
#include "api/user.h"

FILE *tmp_stdin = NULL;

void write_to_stdin(char* data);
void test_method(char *method, char *request);

int main(void) {
	database_init();
	fixtures_run();

	char *texts[4][4] = {
		{ "Tintin", "jaimeenqueter", "Quelle nouvelle aventure palpitante mattend ?", "Le trésor" },
		{ "Milou", "unbelos", "Où est passé mon os préféré ?", "Dans le jardin" },
		{ "Haddock", "millesabords", "Où est mon whisky ?", "Il est caché" },
		{ "Castafiore", "ahjeris", "Où sont mes bijoux ?", "Dans la boite" },
	};

	user_t user, user_bis, user_ter;

	for (int i = 0; i < 4; i++) {
		char *hashed_password = hash(texts[i][1]);
		char *hashed_answer = hash(texts[i][3]);

		user = user_select_with_password(texts[i][0], hashed_password);
		user_bis = user_select_with_answer(user.username, hashed_answer);
		user_ter = user_select_with_token(user_bis.username, user_bis.token);

		printf("%i => %s|%s|%s|%s\n", i + 1, user.username, hashed_password, hashed_answer, user.token);
		printf("%i => %s|%s\n", i + 1, user_bis.username, user_bis.token);
		printf("%i => %s|%s\n", i + 1, user_ter.username, user_ter.token);

		if (i == 3) {
			char *request = malloc(sizeof(char) * 1000);
			sprintf(request, "username=Castafiore&token=%s&new_password=ok", user_ter.token);
			printf("%s\n", request);
			test_method("PUT", request);

			user_free(user_ter);
			user_ter = user_select_with_password(texts[i][0], "ok");
			printf("\n%s\n", user_ter.token);

			strcpy(request, "");
			sprintf(request, "username=Castafiore&token=%s&new_password=%s", user_ter.token, hashed_password);
			printf("%s\n", request);
			test_method("PUT", request);

			free(request);
		}

		user_free(user);
		user_free(user_bis);
		user_free(user_ter);

		free(hashed_password);
		free(hashed_answer);
	}

	test_method("POST", "username=Tournesol&password=ebb5298432585cb17be4033613f35a59&question=Où%20est%20mon%20whisky%20&answer=9f3cdabd069f68a2c2b7f46e91e02519");
	
	test_method("GET", "login_type=password&username=Tintin&secret=2d20c04ce283cc0aa0b2b80ca2cf560f");
	test_method("GET", "login_type=answer&username=Tintin&secret=b0e28a2120575104683591b1c9fde81c");
	
	test_method("GET", "login_type=password&username=zozo&secret=2d20c04ce283cc0aa0b2b80ca2cf560f");

	return 0;
}

void write_to_stdin(char* data) {
    char *dup_data = strdup(data);

	tmp_stdin = tmpfile();
    fwrite(dup_data, 1, strlen(dup_data), tmp_stdin);
    rewind(tmp_stdin);

	free(dup_data);
}

void test_method(char *method, char *request) {
	char length[3];
	sprintf(length, "%d", (int) strlen(request));

	write_to_stdin(request);
	setenv("CONTENT_LENGTH", length, 1);

	if (strcmp(method, "GET") == 0) {
		user_api_get(tmp_stdin);
	} else if (strcmp(method, "POST") == 0) {
		user_api_post(tmp_stdin);
	} else if (strcmp(method, "PUT") == 0) {
		user_api_put(tmp_stdin);
	}

	fclose(tmp_stdin);
}