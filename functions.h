#ifndef FUCTIONS_H
#define FUCTIONS_H

//#include "pipe.h"

#define EXIT_FAILURE -1



 /*
  List of builtin commands, followed by their corresponding functions.
 */
 
 int cmsh_cd(char **args);
int cmsh_help(char **args);
int cmsh_exit(char **args);


void cmsh_loop(char **env);
char *cmsh_read_line(void);
char **cmsh_split_line(char *line);
void cmsh_redirect(char **args);
int cmsh_execute(char **args, char **env);

void redirect_stdin(char **args, int iterator);
void redirect_stdout(char **args, int iterator);
void redirect_stdout_append(char **args, int iterator);
char **remove_args_redirect_symbols(char **args);



#endif 
