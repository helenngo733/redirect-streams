#ifndef REDIR_H
#define REDIR_H

#include <stdbool.h>

void add_character_to_strong(char *str, char c);
void split(char *cmd, char *words[], char delimiter);
bool find_absolute_path(char *cmd, char *absolute_path);

#endif
