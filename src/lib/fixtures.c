#include <sqlite3.h>
#include <stdlib.h>
#include "headers/database.h"
#include "headers/fixtures.h"
#include "headers/user.h"
#include "headers/utils.h"

void fixtures_run(void) {
	int length = 0;
	sqlite3_value ***results = database_select("SELECT COUNT(*) from users", &length);

	if (sqlite3_value_int(results[0][0]) == 0) {
		char *texts[4][4] = {
			{ "Tintin", "jaimeenqueter", "Quelle nouvelle aventure palpitante mattend ?", "Le trésor" },
			{ "Milou", "unbelos", "Où est passé mon os préféré ?", "Dans le jardin" },
			{ "Haddock", "millesabords", "Où est mon whisky ?", "Il est caché" },
			{ "Castafiore", "ahjeris", "Où sont mes bijoux ?", "Dans la boite" },
		};

		for (int i = 0; i < 4; i++) {
			char *hashed_password = hash(texts[i][1]);
			char *hashed_answer = hash(texts[i][3]);

			user_insert(texts[i][0], hashed_password, texts[i][2], hashed_answer);

			free(hashed_password);
			free(hashed_answer);
		}
	}

	sqlite3_value_free(results[0][0]);
	sqlite3_free(results[0]);
	free(results);
}
