#include "main_utils.h"
#include <stdio.h>
#include <string.h>

// set the shell name as the name of the executable file
void set_shell_name(char **name, char *arg) {
    char *found;
    while ((found = strsep(&arg, "/")) != NULL) *name = found;
}

// print the shell prompt
void shell_prompt(char *name) {
    printf("%s> ", name);
}

// print a basic error message
void print_err() {
    fprintf(stderr, "An error has occurred\n");
}
