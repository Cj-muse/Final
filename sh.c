#define FILEOUT 1
#define FILEIN 2
#define FILEAPPEND 3

#include "ucode.c"

int status;
char cmd[256];
char *pipe_commands[16];
int hasPipe;
char *head, *tail;

int number_of_commands;
int io_redirection;
int lpd[2];


int main(int argc, char *argv[]) 
{
	int pid, i = 0;

	while(1)
	{
		printf("Conner's_Shell:>>  ");
		
		//get the user command
		gets(cmd);
		
		if(strcmp(cmd, "logout")==0)
		{
		   printf("logging out...\n");
			exit(0);
			printf("should not be seeing this\n");
		}
		
		// for binary executable command
 		pid = fork(); // fork a child sh process
 		if (pid)
 		{ 
 			// parent sh
			if (!symbol()) // assume at most one & for main sh
 				pid = wait(&status);
			continue; // main sh does not wait if &
 		}
		
				
		// check for pipes
		//checkForPipes();		
		
		// check for redirection
		//checkForRedirection();
		
	 	doCommand(&cmd); 
	}
	exit(0);
	return 0;
}

int symbol()
{
	int i;
	
	while(i < 256)
	{
		if(cmd[i] == '&')
		{
			return 1;
		}
		i++;
	}
	return 0;
}

int doPipe(char *cmdLine, int *pd)
{
	if (pd)// if has a pipe passed in, as WRITER on pipe pd:
	{ 
 		close(pd[0]); dup2(pd[1],1); close(pd[1]);
 	}
 	
 	// divide cmdLine into head, tail by rightmost pipe symbol
 	hasPipe = scanForPipes(cmdLine);
 	if (hasPipe)
 	{
 		//create a pipe lpd;
 		pipe(lpd);
 		pid = fork();
		if (pid)
		{ 
			// parent as READER on lpd:
			close(lpd[1]); dup2(lpd[0],0); close(lpd[0]);
 			doCommand(tail);
		}	
 		else
		 	doPipe(head, lpd);
 	}
}

int scanForPipes(char *cmdLine)
{
	// divide cmdLine into head and tail by rightmost | symbol
	// cmdLine = cmd1 | cmd2 |...|cmdn-1 | cmdn
	// |<------- head --------->| tail |; return 1;
	// cmdLine = cmd1 ==> head=cmd1, tail=null; return 0;
}

int doIoRedirect()
{}

int doCommand(char *cmdLine)
{
	exec(cmdLine);
}


