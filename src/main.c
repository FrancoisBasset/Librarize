#include "lib/headers/database.h"
#include "lib/headers/fixtures.h"
#include "lib/headers/user.h"
#include <stdio.h>
#include <stdlib.h>

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

	return 0;
}
