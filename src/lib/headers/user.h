#ifndef USER_H
#define USER_H
typedef struct user_t {
	int id;
	char *username;
	char *password;
	char *question;
	char *answer;
	char *token;
} user_t;

void user_insert(const char *username, const char *password, const char *question, const char *answer);
user_t user_select_with_username(const char *username);
user_t user_select_with_password(const char *username, const char *password);
user_t user_select_with_answer(const char *username, const char *answer);
user_t user_select_with_token(const char *username, const char *token);
#endif