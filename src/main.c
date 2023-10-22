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
		printf("%s", user.username);
		free(user.username);
	}

	user = user_select_with_answer("Haddock", "9f3cdabd069f68a2c2b7f46e91e02519");
	if (user.username != NULL) {
		printf("%s", user.username);
		free(user.username);
	}

	return 0;
}
