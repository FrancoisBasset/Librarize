#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/headers/database.h"
#include "lib/headers/fixtures.h"
#include "lib/headers/user.h"
#include "api/user.h"

FILE *tmp_stdin = NULL;

void write_to_stdin(char* data);

int main(void) {
	database_init();
	fixtures_run();

	user_t user = user_select_with_password("Tintin", "2d20c04ce283cc0aa0b2b80ca2cf560f");
	if (user.username != NULL) {
		user_t user_bis = user_select_with_token(user.username, user.token);

		printf("%s|%s\n", user_bis.username, user_bis.token);

		free(user.username);
		free(user.token);
		free(user_bis.username);
		free(user_bis.token);
	}

	user_t user2 = user_select_with_answer("Haddock", "9f3cdabd069f68a2c2b7f46e91e02519");
	if (user2.username != NULL) {
		user_t user2_bis = user_select_with_token(user2.username, user2.token);

		printf("%s|%s\n", user2_bis.username, user2_bis.token);

		free(user2.username);
		free(user2.token);
		free(user2_bis.username);
		free(user2_bis.token);
	}

	write_to_stdin("username=Tournesol&password=ebb5298432585cb17be4033613f35a59&question=Où%20est%20mon%20whisky%20&answer=9f3cdabd069f68a2c2b7f46e91e02519");
	setenv("CONTENT_LENGTH", "134", 1);
	user_api_post(tmp_stdin);

	fclose(tmp_stdin);

	return 0;
}

void write_to_stdin(char* data) {
    char *dup_data = strdup(data);

	tmp_stdin = tmpfile();
    fwrite(dup_data, 1, strlen(dup_data), tmp_stdin);
    rewind(tmp_stdin);

	free(dup_data);
}
