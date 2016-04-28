#ifndef PIPE_H
#define PIPE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void execute_pipes(char **env);
int cmsh_piping(char **args, char **env);

#endif
