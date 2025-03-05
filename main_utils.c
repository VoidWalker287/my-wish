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
int w_redirect(char **line) {
    // check for one occurrence redirection character
    char *found = strchr(*line, REDIRECT_CHAR);
    if (!found) {
        return REDIRECT_NONE;
    } else if (strlen(found) == 1 || strchr(found + 1, REDIRECT_CHAR)) {
        return REDIRECT_BAD;
    }

    // split and trim line and file name
    char *file_line = found + 1;
    while (is_delim(*file_line)) file_line++;
    while(is_delim(*(found - 1))) found--;
    *found = '\0';
    char *file_name = strsep(&file_line, COMMAND_DELIM);

    // check for invalid file
    if (file_line) {
        return REDIRECT_BAD;
    }

    // replace stdout file descriptor with output file
    int output_fd = fileno(fopen(file_name, "w"));
    if (output_fd == -1) {
        return REDIRECT_BAD;
    }
    std_out_copy = dup(STDOUT_FILENO);
    int res = dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    return (res != -1) ? REDIRECT_GOOD : REDIRECT_BAD;
}

// restore original file descriptor for stdout
int w_restore(void) {
    int file = dup2(std_out_copy, STDOUT_FILENO);
    close(std_out_copy);
    return file;
}