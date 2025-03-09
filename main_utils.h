#ifndef WISH_MAIN_UTILS_H
#define WISH_MAIN_UTILS_H

#include <stdbool.h>

#define MAX_LINE_LENGTH 200
#define MAX_ARGUMENTS 20
#define COMMAND_DELIM " \n\r\t"
#define REDIRECT_CHAR '>'
#define REDIRECT_NONE   0
#define REDIRECT_GOOD   1
#define REDIRECT_BAD    (-1)

void set_shell_name(char **name, char *arg);
void shell_prompt(char *name);
void print_err();
bool is_delim(char c);
int w_redirect(char *line);
int w_restore(void);

#endif //WISH_MAIN_UTILS_H
