int pid, child[3], status;
int stdin, stdout;
int s0pid, s1pid;
char *TTY0 = "/dev/tty0";
//char *TTYS0 = "/dev/ttyS0";
//char *TTYS1 = "/dev/ttyS1";
char * ttys[3] = {"/bin/login /dev/tty0","/bin/login /dev/ttyS0","/bin/login /dev/ttyS1"};

#include "ucode.c"  //<========== AS POSTED on class website

main(int argc, char *argv[])
{
   //1. // open /dev/tty0 as 0 (READ) and 1 (WRTIE) in order to display messages
   close(0);
   close(1);
   
   //opening the the tty0 file for both reading and writing 
   stdin = open(TTY0, O_RDONLY);
   stdout = open(TTY0, O_WRONLY);
   
	login();
   
   exit(0);
}       

int login()
{  
   int i = 0; 
      
   for (i=0; i < 3; i++)
   {
      child[i] = fork();
      if(!child[i])
      {
         //child proc exec
       	printf("CM_INIT : forking a login task for console\n"); 
         exec(ttys[i]);
      }
   }
   parent();
}
      
int parent()
{   
   int kid;
   int i = 0;
	while(1)
	{
		printf("CMINIT : parent waiting ...\n");

   	pid = wait(&status);  // wait for child to die
   
      for(i = 0; i < 3; i++)
      {
   	   if (pid == child[i])
   	   {
            //fork another login child
            printf("fork another login child");
            kid = fork();
            if(kid)
            {
            	//continue;
               parent();
            }
            else
            {  
               exec(ttys[i]);
            }
   	   }
       	else
       	{
         	printf("INIT : buried an orphan child %d\n", pid);
         }
      }
	}
}
