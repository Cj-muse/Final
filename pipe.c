#include "pipe.h"

char head[64][64]; 
char tail[64][64];

int cmsh_piping(char **args, char **env)
{
	int i = 0, j = 0, u = 0, x = 0;
	int pipe = 0;
	int pid, status;
	
	while (args[i])
	{
		//printf("args[i] = %s \n", args[i]);
		if (strcmp(args[i],"|") == 0)
		{
			pipe = 1;
			//printf("Enter if correctly\n");
			//printf("i = %d \n", i);
			// pipe command given, split into head and tail
			for (j = 0; j < i; j++) //head
			{
				strcpy(head[j], args[j]);
			}
			*head[j]=0;

			for (j = i, u = 0; args[j+1]; j++, u++) //tail
			{
				//printf("In tail for loop\n");
				//printf("args[j+1] = %s \n", args[j+1]);
				strcpy(tail[u], args[j+1]);
			}
			*tail[u]=0;
		}
		i++;
	}
	
	//printf("pipe = %d \n", pipe);
	if (pipe == 0)
	{
		//printf("No Pipes\n");
		return 0;
	}
      
    	pid = fork();
    	
    	printf("pid = %d\n", pid);
    	if (pid) // parent
    	{
    		printf("parent is waiting \n");
    		pid = wait(&status);
    		printf("parent is awake \n");
    	}
    	else // child
    	{
	    	printf("Creating pipes...\n");
    		execute_pipes(env);
    		printf("you should not be reading this\n");
	}
	//printf("returning one from pipes\n");
	return 1;
}

void execute_pipes(char **env)
{
	pid_t;
	int pd[2];       // 2 integers
      char buf[1024];
      int r, n, pid;
  	int status;
  	int i = 0, j = 0, u = 0;

  	char *test1[3]={"cat","pipe.c"};
	char *test2[3] = {"grep", "char"};
  	
	
	for (i = 0; i < 5; i++)
	{
		printf("tail[%d] = !!!%s!!!\n", i , tail[i]);
		printf("head[%d] = !!!%s!!!\n", i , head[i]);
	}
	//pause	
  	getchar();
  
  	
     pipe(pd);  // system call to create a pipe' pd[0]=READ, pd[1]=WRITE
     printf("pd=[%d, %d]\n", pd[0], pd[1]);
 
	 // fork a child process, which has the SAME pd[ ]
     pid = fork();   
       
     if(pid == -1)
     {
		perror("fork");
      	exit(1);
     }
     
	 printf("pid = %d \n", pid);
     if (pid)
     {  
		printf ("test 123!!!\n");     // parent as pipe WRITER
 		close(pd[0]);// WRITER MUST close pd[0]
      	close(1);    // close 1
      	dup(pd[1]);  // replace 1 with pd[1]
		
     		if((status = execvp(head[0], head)) <0)  // change image to cmd1
		{
			getchar();
			printf("fAIL!!\n");
		}	
     		//printf("parent exc resuilt = %d\n", status);  
		     
		//if ((status = execvp(test1[0],test1))<0)
		//	getchar();
	}
    else
    {            // child as pipe READER
   		close(pd[1]); // READER MUST close pd[1]
      	close(0);  
      	dup(pd[0]);   // replace 0 with pd[0]
		//printf("in child process for execution \n");
      	if ((status = execvp(tail[0], tail)) < 0)
		
		//if ((status = execvp("bin", tail)) < 0)
		{
			printf("Child process in pipe failed!!\n");
			exit(1);
		}
		   // change image to cmd2
		//if((status = execvp(test2[0],test2))<0 )
		//	printf("chils!!!!\n");			
		printf("child exc resuilt = %d\n",status);  
		
    }    
     printf("never should see no matter what\n");   
}




