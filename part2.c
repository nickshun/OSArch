#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int gCounter = 0;

int main ()
{
   int pipefd[2];
   
   if (pipe(pipefd) == -1)
   {
      printf("pipe failed\n");
      exit(0);
   }
   
   for (int i = 0; i < 5; ++i)
   {
      pid_t pid = fork();
      
      if (pid == 0) //if child
      {
         while (gCounter != 100)
         {
            gCounter++;
            printf("new counter: %d PID: %d\n", gCounter, getpid());
         }
         
         break;
      }
      else if (pid > 0) //if parent
      {
      }
   }
   
   wait(NULL);
   
   printf("--END PROCESS--\n");
   
	return 0;
}
