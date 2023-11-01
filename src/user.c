#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/user.h"

int main(void) {
	const char *method = getenv("REQUEST_METHOD");
	
	if (strcmp(method, "GET") == 0) {
		user_api_get();
	} else if (strcmp(method, "POST") == 0) {
		user_api_post(stdin);
	}

	return 0;
}
