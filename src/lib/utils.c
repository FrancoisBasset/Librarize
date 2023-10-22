#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bsd/md5.h>
#include <uuid/uuid.h>
#include "headers/utils.h"

char* hash(char *text) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX mdContext;

	MD5Init(&mdContext);
    MD5Update(&mdContext, (unsigned char*) text, strlen(text));
    MD5Final(digest, &mdContext);

	char *result = malloc(sizeof(char) * 33);
	strcpy(result, "");

    for(int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(result,"%s%02x", result, digest[i]);
    }
    
	return result;
}

char* get_uuid(void) {
	uuid_t binuuid;
	uuid_generate_random(binuuid);

	char *uuid = malloc(sizeof(char) * 37);
	uuid_unparse_lower(binuuid, uuid);

	return uuid;
}