//
// Created by Kyle Smith on 2/19/25.
//

#ifndef WISH_MAIN_UTILS_H
#define WISH_MAIN_UTILS_H

#include <stdbool.h>

#define MAX_LINE_LENGTH 200
#define MAX_ARGUMENTS 20
#define COMMAND_DELIM " \n\r\t"

void set_shell_name(char **name, char *arg);
void shell_prompt(char *name);
void print_err();

#endif //WISH_MAIN_UTILS_H
