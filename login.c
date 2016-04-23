#include "ucode.c"
char *tty;
int stdin, stdout;
char input_username[32], input_password[32];
char file_username[32], file_password[32];
char buffer[256];
int fd;

typedef struct user{

   // struct definition for a user following the /ect/password format..
  	//  username:password:gid:uid:fullname:HOMEDIR:program
   char username[32];
   char password[32];

   int gid;
   int uid;
   
   char fullname[64];
   char homedir[64];
   char program[64];
   
}USER;

USER * users[10];

void getUsersFromFile()
{
	char *string;
	char *line;
	char *field;
	int i = 0, j = 0; 

	while (read(fd, buffer, 256))// still bytes to be read
	{
		string = buffer;
		//parse whats been read in into seperate lines
		 line  = strchr(string, '\n');
	
	    while (line != 0)
	    {
	        // String to scan is in string..
	        *line++ = '\0';
	        printf("a = %s\n", string);
	        field = strtok(string, ":");
	        
	        while (field != 0)
	        {
	            printf("field = %s\n", field);
	            switch(j)
         		{
            		case 1: strcpy(users[i]->password, field); break;
            		case 2: users[i]->gid = field;             break;
            		case 3: users[i]->uid = field;             break;
            		case 4: printf("case 4\n");strcpy(users[i]->fullname, field);break;
            		case 5: strcpy(users[i]->homedir, field);  break;
            		case 6: strcpy(users[i]->program, field);  break;
            		default: break;
         		}	
         		printf("users[%d]: %d\n",i, users[i]);
         		printf("users[%d]->username: %s", i, users[i]->username);
	            field = strtok(0, ":");
	            j++;
	        }
	        string = line;
	        line = strchr(string, '\n');
	        i++;
	    }
	}
}   

void initUsers()
{
	int i = 0;
		
	for(i = 0; i<10; i++)
	{
		users[i] = (USER*)malloc(sizeof(USER));
	}
}

void printUsers()
{
	int i = 0;
	printf("users[%d]: %d\n",i, users[i]);
	while(users[i])
	{
		printf("users[%d]\n",i);
		printf("username: %s\n", users[i]->username);
		printf("password: %s\n", users[i]->password);
		printf("gid: %d\n", users[i]->gid);
		printf("uid: %d\n", users[i]->uid);
		printf("fullname: %s\n", users[i]->fullname);
		printf("homedir: %s\n", users[i]->homedir);
		printf("program: %s\n", users[i]->program);
		i++;
	}
}


main(int argc, char *argv[])   // invoked by exec("login /dev/ttyxx")
{
   int i;
	tty =  argv[1];
	
	//initialize user array
	initUsers();
	
	close(0); close(1); close(2); // login process may run on different terms

	// open its own tty as stdin, stdout, stderr
	stdin = open(tty, O_RDONLY);
   stdout = open(tty, O_WRONLY);
   
	settty(tty);   // store tty string in PROC.tty[] for putc()

	//NOW we can use printf, which calls putc() to our tty
   printf("CM_LOGIN : open %s as stdin, stdout, stderr\n", tty);

	signal(2,1);  // ignore Control-C interrupts so that 
                // Control-C KILLs other procs on this tty but not the main sh

	while(1)
	{
      printf("Login: ");		//1. show login:           to stdout
      gets(input_username);         //2. read user nmae        from stdin
   	printf("Password: ");   //3. show passwd:
      gets(input_password);    		//4. read user passwd
      
     	// open the authentication file for reading 
     	//fd = open("/etc/passwd", O_RDONLY);
     	fd = open("/bin/accounts", O_RDONLY);
     	
     	//read in each credetial and check it against what the user entered
     	//  username:password:gid:uid:fullname:HOMEDIR:program
      getUsersFromFile();
      
      printUsers();

     	//5. verify user name and passwd from /etc/passwd file
     	while (users[i])
     	{
     	   if ( strcmp(users[i]->username, input_username) == 0 && 
     	        strcmp(users[i]->password, input_password) == 0)
     	   {
     	      //Successfull login.
     	      //setuid to user uid.
         	//chdir to user HOME directory.
         	//exec to the program in users's account
         	return;
     	   }
     	   i++;
     	}
     	printf("login failed, try again \n");
   }
}
