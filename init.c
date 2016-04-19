int pid, child, status;
int stdin, stdout;
int s0pid, s1pid;

#include "ucode.c"  //<========== AS POSTED on class website

main(int argc, char *argv[])
{
   //1. // open /dev/tty0 as 0 (READ) and 1 (WRTIE) in order to display messages
   close(0);
   close(1);
   
   //opening the the tty0 file for both reading and writing 
   stdin = open("/dev/tty0", O_RDONLY);
   stdout = open("/dev/tty0", O_WRONLY);
   
   //2. // Now we can use printf, which calls putc(), which writes to stdout
	printf("CM_INIT : forking a login task for console\n"); 
   child = fork();
   
   if (child)
   {
      parent();
	}
	else             // login task
   {
   	login();
   }
}       

int login()
{
   exec("login /dev/tty0");
}
      
int parent()
{   
   // Set up serial ports
   s0pid = fork();  
   printf("s0pid after fork: %d\n",s0pid);
   //printf("");
   
   if(s0pid)
   {
      // Parent
      s1pid = fork(); 
      if(s1pid)
      {
         return;
      }
      else
      {
         // Child: open the first termial ttyS1
         // set up stdin stdout, and execute login program
         close(0);
         close(1);
         stdin = open("/dev/ttyS1", O_RDONLY);
         stdout = open("/dev/ttyS1", O_WRONLY);
         exec("login /dev/ttyS1");
      }  
   }
   else
   {
      // Child: open the second termial ttyS0,
      // set up stdin stdout, and execute login program
      close(0);
      close(1);
      stdin = open("/dev/ttyS0", O_RDONLY);
      stdout = open("/dev/ttyS0", O_WRONLY);
      exec("login /dev/ttyS0");
   }

	while(1)
	{
		printf("CMINIT : parent waiting ...\n");

   	pid = wait(&status);  // wait for child to die

   	if (pid == child)
   	{
         //fork another login child
         printf("fork another login child");
         child = fork();
         if(child)
         {
            parent();
         }
         else
         {
            login();
         }
   	}
    	else
    	{
      	printf("INIT : buried an orphan child %d\n", pid);
      }
	}
}
