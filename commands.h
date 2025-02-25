#ifndef WISH_COMMANDS_H
#define WISH_COMMANDS_H

#include <stdbool.h>

bool w_execute(char *args[], int num_args);
void init_path();
void clear_path();

#endif //WISH_COMMANDS_H
