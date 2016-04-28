


char **parseRedirectionOnSpaces(char *line)
{
   char *token;
   char *array[10];
   int i = 0;
   
   //split line on spaces
   array[i] = strtok(line, " ");
   printf("array[0]: %s \n", array[0]);
   i = 1;
   
   while(array[i-1] != 0)
   {
      array[i] = strtok(0, " "); 
      printf("array[%d]: %s \n", i, array[i]);
      i++;
   }

   return array;
}

int doRedirection(char **parsedLine)
{

}

int scanForRedirection(char *line)
{
   int i = 0;
   char **parsedLine;
   while(line[i])
   {
      printf("in while\n");
      printf("line[i] = %c \n", line[i]);
      if(line[i] == '<' || line[i] == '>') 
      {
         parsedLine = parseRedirectionOnSpaces(line);
         doRedirection(parsedLine);
         printf("performed redirection\n");
         return;
      }
      i++;
   }
   printf("no redirection");
   return;
}

