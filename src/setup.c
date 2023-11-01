#include <stdio.h>
#include <stdlib.h>
#include "lib/headers/database.h"
#include "lib/headers/fixtures.h"

int main(void) {
	database_init();
	fixtures_run();

	return 0;
}
