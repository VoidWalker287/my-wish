#include "main_utils.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *shell_name;
    FILE *input_file;

    if (argc == 1) {
        input_file = stdin;
        set_shell_name(&shell_name, argv[0]);
        shell_prompt(shell_name);
    } else if (argc != 2 || !(input_file = fopen(argv[1], "r"))) {
        print_err();
        exit(EXIT_FAILURE);
    }

    init_path();

    char *line;
    size_t line_len = MAX_LINE_LENGTH;
    char *args[MAX_ARGUMENTS];
    while (getline(&line, &line_len, input_file) != EOF) {
        // parse line into array of arguments
        char *current_arg;
        int num_args = 0;
        line[strlen(line) - 1] = '\0';
        while ((current_arg = strsep(&line, COMMAND_DELIM)) != NULL) {
            args[num_args] = current_arg;
            num_args++;
        }
        // search and execute commands
        if (!w_execute(args, num_args)) {
            print_err();
        }
        // print prompt in interactive mode
        if (argc == 1) {
            shell_prompt(shell_name);
        }
    }
    clear_path();
    exit(EXIT_SUCCESS);
}
