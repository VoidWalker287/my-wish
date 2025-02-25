#include "commands.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char *paths[50]; // global paths array for storing directories

typedef struct {
    char *name;
    bool (*execute)(char *args[], int num_args);
} command;

// exit the shell
bool w_exit(char *args[], int num_args) {
    if (num_args != 1) {
        return false;
    } else {
        exit(0);
    }
}

// change the current working directory
bool w_cd(char *args[], int num_args) {
    if (num_args != 2) {
        return false;
    }
    int res = chdir(args[1]);
    return res == 0;
}

// set the path
bool w_path(char *args[], int num_args) {
    int i = 0;
    while (paths[i] != NULL) {
        free(paths[i]);
        paths[i] = NULL;
        i++;
    }
    for (i = 1; i < num_args; i++) {
        size_t path_len = strlen(args[i]) + 1;
        paths[i - 1] = (char *) malloc(path_len * sizeof(char));
        strcpy(paths[i - 1], args[i]);
        paths[i - 1][path_len - 1] = '\0';
    }
    return true;
}

// print the current working directory
bool w_pwd(char *args[], int num_args) {
    if (num_args == 1) {
        char buf[100];
        getcwd(buf, 100);
        printf("%s\n", buf);
        return true;
    } else {
        return false;
    }
}

// print the current path
bool w_pp(char *args[], int num_args) {
    if (num_args != 1) {
        return false;
    } else {
        int i = 0;
        while (paths[i] != NULL) {
            printf(i == 0 ? "%s" : " %s", paths[i]);
            i++;
        }
        if (i > 0) {
            printf("\n");
        }
        return true;
    }
}

// execute external command in child process
bool run_extern(char *file_path, char *args[], int num_args) {
    // construct new arguments list for use with execv
    char *extern_args[num_args + 1];
    extern_args[0] = file_path;
    for (int i = 1; i < num_args; i++) {
        extern_args[i] = args[i];
    }
    extern_args[num_args] = NULL;

    // execute external command in child process
    int pid = fork();
    if (pid < 0) {
        // error
        return false;
    } else if (pid == 0) {
        // child
        execv(extern_args[0], extern_args);
        exit(EXIT_FAILURE);
    } else {
        // parent
        wait(NULL);
        return true; // ignore exit status of external command
    }
}

// search for external commands
bool w_extern(char *args[], int num_args) {
    int i = 0;
    while (paths[i] != NULL) {
        size_t file_path_len = strlen(paths[i]) + 1 + strlen(args[0]);
        char *file_path = (char *) malloc(file_path_len * sizeof(char));
        strcpy(file_path, paths[i]);
        strcat(file_path, "/");
        strcat(file_path, args[0]);
        if (access(file_path, X_OK) == 0) {
            bool res = run_extern(file_path, args, num_args);
            free(file_path);
            return res;
        }
        free(file_path);
        i++;
    }
    return false;
}

// list of structs for builtin commands
command commands[] = {
        {"exit", &w_exit},
        {"cd",   &w_cd},
        {"path", &w_path},
        {"pwd",  &w_pwd},
        {"pp",   &w_pp},
        {NULL , &w_extern} // final command will always check external commands
};

// searches for builtin and external commands, returns true on successful execution
bool w_execute(char *args[], int num_args) {
    int i = 0;
    while (commands[i].name != NULL && strcmp(args[0], commands[i].name) != 0) i++;
    return commands[i].execute(args, num_args);
}

// initializes the path with default values
void init_path() {
    char *path_args[] = {"path", "/usr", "/bin"};
    w_path(path_args, 3);
}

// frees the entire path
void clear_path() {
    char *path_args[] = {"path"};
    w_path(path_args, 1);
}
