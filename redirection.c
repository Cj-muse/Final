
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

int redirect_std(char **args, int iterator)
{
	int fileHandle = -1;
	
	printf("attempting to open: %s\n",args[iterator]);
	printf("iterator -1: %s\n",args[iterator - 1]);
	
	if (strcmp(args[iterator - 1],"<") == 0)
	{		
		close(0);	
		fileHandle = open(args[iterator], O_RDONLY);
	}
	else if (strcmp(args[iterator - 1],">") == 0)
	{
		close(1);
		//, S_IWUSR | S_IRUSR not available in mtx
		fileHandle = open(args[iterator], O_WRONLY | O_CREAT); 
	}
	else
	{
		close(1);	
		fileHandle = open(args[iterator], O_WRONLY | O_APPEND | O_CREAT);
	}
		
	return fileHandle;
}


int doRedirection(char **parsedLine)
{
	int i = 0;
	char *line;
	int fd = -1;
	
	printf("made it this far\n");
	
	while(parsedLine[i])
	{
		printf("parsedLine[i]: %s\n", parsedLine[i]);
		if (strcmp(parsedLine[i],"<") == 0 || strcmp(parsedLine[i],">") == 0 
			|| strcmp(parsedLine[i],">>") == 0)
		{
			fd = redirect_std(parsedLine, i+1);
		
			printf("fd = %d\n", fd);
			if (fd != -1) // redirect has occured
			{		   
				//about to 
			   //dup(fd);
			   line = reappendParsedLine(parsedLine);
			   exec(line);
			}
		}
		i++;
	}
		
	//exec(line);
}


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

