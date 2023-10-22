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
		char *hash1 = hash("jaimeenqueter");
		char *hash2 = hash("Le trésor");
		char *hash3 = hash("unbelos");
		char *hash4 = hash("Dans le jardin");
		char *hash5 = hash("millesabords");
		char *hash6 = hash("Il est caché");
		char *hash7 = hash("ahjeris");
		char *hash8 = hash("Dans la boite");

		user_insert("Tintin", hash1, "Quelle nouvelle aventure palpitante mattend ?", hash2);
		user_insert("Milou", hash3, "Où est passé mon os préféré ?", hash4);
		user_insert("Haddock", hash5, "Où est mon whisky ?", hash6);
		user_insert("Castafiore", hash7, "Où sont mes bijoux ?", hash8);

		free(hash1);
		free(hash2);
		free(hash3);
		free(hash4);
		free(hash5);
		free(hash6);
		free(hash7);
		free(hash8);
	}

	sqlite3_value_free(results[0][0]);
	sqlite3_free(results[0]);
	free(results);
}
