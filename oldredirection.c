#include "functions.h"

#define FILEOUT 1
#define FILEIN 2
#define FILEAPPEND 3

int stdin, stdout, stderr;

int 	REDIRECT_TYPE = 0;

void cmsh_loop(char **env)
{
	char *line;
	char **args;
	int status;

	do 
	{
    		printf("> ");
		line = cmsh_read_line();
	     args = cmsh_split_line(line);
		status = cmsh_execute(args, env);

	      free(line);
	      free(args);
	}  while (status);
}


char *cmsh_read_line(void)
{
	char *line = NULL;
  	int bufsize = 0; // have getline allocate a buffer 
  	getline(&line, &bufsize, stdin);
  	return line;
}


//returns an array of potiners to chars  essentially a char [x][y]
#define CMSH_TOK_BUFSIZE 64
#define CMSH_TOK_DELIM " \t\r\n\a"
char **cmsh_split_line(char *line)
{
	int bufsize = CMSH_TOK_BUFSIZE, element = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) 
	{
		fprintf(stderr, "cmsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, CMSH_TOK_DELIM);
	while (token != NULL) 
	{
		tokens[element] = token;
		element++;
		if (element >= bufsize) 
		{
			bufsize += CMSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) 
			{
				fprintf(stderr, "cmsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, CMSH_TOK_DELIM);
	}
  	tokens[element] = NULL;
  	return tokens;
}

void cmsh_redirect(char **args)
{
	int i = 0;

	while (args[i])
	{
		if (strcmp(args[i],"<") == 0)
		{
			redirect_stdin(args, i+1);		
		}
		else if (strcmp(args[i],">") == 0)
		{
			redirect_stdout(args, i+1);
		}
		else if (strcmp(args[i],">>") == 0)
		{
			redirect_stdout_append(args, i+1);
		}
		i++;
	}
}

void redirect_stdin(char **args, int iterator)
{
	int fileHandle;
	
	close(0);	
	fileHandle = open(args[iterator], O_RDONLY);
	REDIRECT_TYPE = 1;		
}

void redirect_stdout(char **args, int iterator)
{
	FILE *fileHandle;
	
	close(1);
	fileHandle = open(args[iterator], O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
	REDIRECT_TYPE = 2;	
}

void redirect_stdout_append(char **args, int iterator)
{
	FILE *fileHandle;
	
	close(1);
	fileHandle = open(args[iterator], O_WRONLY | O_APPEND | O_CREAT);
	REDIRECT_TYPE = 3;	
}

char **remove_args_redirect_symbols(char **args)
{
	int i = 0;
	
	while(args[i])
	{
		if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
		{
			args[i] = NULL;
		}
		i++;
	}
	return args;
}

int cmsh_execute(char **args, char **env)
{
	int i;

  	if (args[0] == NULL) 
  	{
    		// An empty command was entered.
    		return 1;
  	}

  	for (i = 0; i < cmsh_num_builtins(); i++) 
  	{
    		if (strcmp(args[0], builtin_str[i]) == 0) 
    		{
     		return (*builtin_func[i])(args);
    		}
  	}

  return cmsh_launch(args, env);
}


int cmsh_launch(char **args, char **env)
{
	pid_t pid, wpid;
  	int status = 0;
	int pipe = 0;
	
	
		pipe = cmsh_piping(args, env);
		
		if (pipe > 0)
		{
			printf("Piping complete \n");
			return 1;
		}
	
  	pid = fork();
	  	
  	if (pid == 0) 
  	{
		cmsh_redirect(args);
		// check for redirection
		if (REDIRECT_TYPE > 0)
		{
			args = remove_args_redirect_symbols(args);
		}
   		// Child process
    		if (execvp(args[0], args) == -1) 
    		{
     		perror("cmsh here?");
     	}
     	return 0;
    		exit(EXIT_FAILURE);
  	} 
  	else if (pid < 0) 
  	{
    		// Error forking
    		perror("cmsh or here?");
  	} 
  	else 
  	{
    		// Parent process
    		do {
    	  		wpid = waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}
	printf("child exit status = %d\n", status);
  	return 1;
}



/*
  Builtin function implementations.
*/
int cmsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "cmsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("ooo");
    }
  }
  return 1;
}

int cmsh_help(char **args)
{
  int i;
  printf("CM shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < cmsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}







