
//int stdin, stdout, stderr;
int 	REDIRECT_TYPE = 0;


char **parseRedirectionOnSpaces(char *line)
{
   char *token;
   char *array[10];
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
		printf("parsedLine[%d]: %s \n", i, &(*parsedLine)[i]);
		while(parsedLine[i][j])
		{
			printf("parsedLine[%d][%d] = %c \n", i, j, parsedLine[i][j]);
			line[k] = parsedLine[i][j];
			printf("line[%d] = %c\n", k, line[k]);
			k++;
			j++;
		}
		j = 0;
		i++;
	}
	return line;
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
	int fileHandle;
	
	close(1);
	//, S_IWUSR | S_IRUSR not available in mtx
	fileHandle = open(args[iterator], O_WRONLY | O_CREAT); 
	REDIRECT_TYPE = 2;	
}

void redirect_stdout_append(char **args, int iterator)
{
	int fileHandle;
	
	close(1);
	fileHandle = open(args[iterator], O_WRONLY | O_APPEND | O_CREAT);
	REDIRECT_TYPE = 3;	
}

int doRedirection(char **parsedLine)
{
	int i = 0;
	char *line;
	
	printf("made it this far\n");
	//printf("parsedLine[0] = %s\n", parsedLine[0]);
	
	while(parsedLine[i])
	{
		if (strcmp(parsedLine[i],"<") == 0)
		{
			redirect_stdin(parsedLine, i+1);		
		}
		else if (strcmp(parsedLine[i],">") == 0)
		{
			redirect_stdout(parsedLine, i+1);
		}
		else if (strcmp(parsedLine[i],">>") == 0)
		{
			redirect_stdout_append(parsedLine, i+1);
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
   char **parsedLine;
   
   while(line[i])
   {
      if(line[i] == '<' || line[i] == '>') 
      {
         parsedLine = parseRedirectionOnSpaces(line);
         //printf("parsedLine[0] = %s\n", parsedLine[0]);
         doRedirection(&parsedLine);
         //printf("performed redirection\n");
         return 1;
      }
      i++;
   }
   //printf("no redirection");
   return 0;
}

