#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

int gCounter = 0;

#define NUM_CHILDREN 5

void *parent(void* PID);
void *child(void* PID);

int main ()
{
   pthread_t* parentThread;
   pthread_t children[NUM_CHILDREN];
   
   int rc;
   rc = pthread_create(parentThread, NULL, parent, (void*)0);
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
   }
   
   for (long i = 1; i <= NUM_CHILDREN; ++i)
   {
      rc = pthread_create(children + (i-1), NULL, child, (void*) i);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   
   pthread_exit(NULL);
   
   
   
   
   /*for (int i = 0; gCounter != 100; ++i)
   {
      if (i != 0)
      {
         gCounter++;
         printf("New Value: %d PID: %d\n", gCounter, i);
      }
      
      if (i > NUM_CHILDREN)
      {
         i = 0;
      }
   }*/
   
	return 0;
}

void *parent(void* pid)
{
   int PID = (int)pid;
   
   printf("parent\n");
   
   pthread_exit(NULL);
}

void *child(void* pid)
{
   int PID = (int)pid;
   
   printf("child\n");
   
   pthread_exit(NULL);
}
