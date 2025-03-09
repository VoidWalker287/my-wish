#include "main_utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int std_out_copy;

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

// determine if a character is a delimiter for manual trimming
bool is_delim(char c) {
    char *delims = COMMAND_DELIM;
    size_t len = strlen(COMMAND_DELIM);
    for (int i = 0; i < len; i++) {
        if (c == delims[i]) {
            return true;
        }
    }
    return false;
}

// perform output redirection
int w_redirect(char *line) {
    char *redirect = strchr(line, REDIRECT_CHAR);
    if (!redirect) {
        return REDIRECT_NONE;
    } else if (strlen(redirect) < 2 || strchr(redirect + 1, REDIRECT_CHAR)) {
        return REDIRECT_BAD;
    }

    char *file_name = redirect + 1;

    // trim arguments whitespace
    while (redirect > line && is_delim(*(redirect - 1))) {
        redirect--;
    }
    if (redirect == line) {
        return REDIRECT_BAD;
    }
    *redirect = '\0';

    // trim file name leading whitespace
    while (strlen(file_name) && is_delim(*file_name)) {
        file_name++;
    }
    // trim file name trailing whitespace
    char *file_end = file_name + strlen(file_name) - 1;
    while (file_end > file_name && is_delim(*(file_end - 1))) {
        file_end--;
    }
    *file_end = '\0';

    if (!strlen(file_name)) {
        return REDIRECT_BAD;
    }

    // check for whitespace in file name (bad redirect call)
    for (int i = 0; i < strlen(file_name); i++) {
        if (is_delim(file_name[i])) {
            return REDIRECT_BAD;
        }
    }

    // perform redirect
    std_out_copy = dup(STDOUT_FILENO);
    int redirect_fd = fileno(fopen(file_name, "w"));
    if (redirect_fd == -1) {
        return REDIRECT_BAD;
    }
    int res = dup2(redirect_fd, STDOUT_FILENO);
    close(redirect_fd);
    return (res == -1) ? REDIRECT_BAD : REDIRECT_GOOD;
}

// restore original file descriptor for stdout
int w_restore(void) {
    int file = dup2(std_out_copy, STDOUT_FILENO);
    close(std_out_copy);
    return file;
}