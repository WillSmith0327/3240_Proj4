#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    char* envp[] = {"LC_ALL C", NULL};
    int i, rc;
    int thePipe[2]; 
    alarm(60);

    if(strcmp(argv[1], "-hw") == 0)
    {
      printf("%s \n", "hello world");
      exit(0);
    } 

    pipe(thePipe);

    rc = fork();
    if (rc == -1)
    {
        printf("fork failed\n");
	     exit(-1);
    }
    if (rc == 0)  /* child */
    {
        
        dup2(thePipe[0], STDIN_FILENO);
        close(thePipe[0]);
        close(thePipe[1]);

        char *sortArgV[argc + 1];
        sortArgV[0] = "/usr/bin/sort";
        for(int i = 1; i < argc + 1; i++)
        {
            sortArgV[i] = argv[i + 1];
        }
        sortArgV[argc + 1] = NULL;

	     execve("/usr/bin/sort", sortArgV, envp);
	     exit(0);
    }
    /* parent */
    close(thePipe[0]);
    char *fileName = argv[1];
    FILE *file = fopen(fileName, "r");
    char *badWord = "foobar";
    char buf[100];
    
    while(fgets(buf, sizeof(buf), file))
    {
      char *wordReturn = (strstr(buf,badWord));
      if (wordReturn == NULL)
      {
         dprintf(thePipe[1], "%s", buf);         
      }
    }
   return 0;
}
