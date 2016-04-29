
//int stdin, stdout, stderr;
int 	REDIRECT_TYPE = 0;
char *array[10];

char **parseRedirectionOnSpaces(char *line)
{
   char *token;

   int i = 0;
   
   //split line on spaces
   array[i] = strtok(line, " ");
   
   i = 1;
   while(array[i-1] != 0)
   {
      array[i] = strtok(0, " "); 
      i++;
   }
   
   return array;
}

char *reappendParsedLine(char **parsedLine)
{
	int i = 0, j = 0, k = 0;
	char *line;
	
	while(parsedLine[i])
	{
	   strcat(line, parsedLine[i]);	   
	}
	printf("line: %s\n", line);  
	
	return line;
}

int redirect_stdin(char **args, int iterator)
{
	int fileHandle;
	
	close(0);	
	fileHandle = open(args[iterator], O_RDONLY);
	REDIRECT_TYPE = 1;		
	return fileHandle;
}

int redirect_stdout(char **args, int iterator)
{
	int fileHandle;
	
	close(1);
	//, S_IWUSR | S_IRUSR not available in mtx
	fileHandle = open(args[iterator], O_WRONLY | O_CREAT); 
	REDIRECT_TYPE = 2;
   return fileHandle;	
}

int redirect_stdout_append(char **args, int iterator)
{
	int fileHandle;
	
	close(1);
	fileHandle = open(args[iterator], O_WRONLY | O_APPEND | O_CREAT);
	REDIRECT_TYPE = 3;	
	return fileHandle;
}

int doRedirection(char **parsedLine)
{
	int i = 0;
	char *line;
	int fd = 0;
	
	printf("made it this far\n");
	
	while(parsedLine[i])
	{
		if (strcmp(parsedLine[i],"<") == 0)
		{
			fd = redirect_stdin(parsedLine, i+1);		
		}
		else if (strcmp(parsedLine[i],">") == 0)
		{
			fd = redirect_stdout(parsedLine, i+1);
		}
		else if (strcmp(parsedLine[i],">>") == 0)
		{
			fd = redirect_stdout_append(parsedLine, i+1);
		}
		if (fd) // redirect has occured
		{		   
		   dup(fd);
		   
		   exec(line);
		}
		i++;
	}
	
	line = reappendParsedLine(parsedLine);
	
	//exec(line);
}

void restablishStandardIO()
{}

int scanForRedirection(char *line)
{
   int i = 0;
   int child;
   char **parsedLine;
      
   while(line[i])
   {
      if(line[i] == '<' || line[i] == '>') 
      {
         parsedLine = parseRedirectionOnSpaces(line);
         child = fork();
         if(!child)
         {
            doRedirection(parsedLine);
            //printf("performed redirection\n");
            return 1;
         }
         else
         {
            wait();
         }
      }
      i++;
   }
   //printf("no redirection");
   return 0;
}

