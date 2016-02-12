#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t counter;

int gCounter = 0;

#define NUM_CHILDREN 5
#define FIFO_NAME "synchPipe"

int pipeFD;

void *parent(void* PID);
void *child(void* PID);

int main ()
{
   sem_init(&counter, 0, 1);
   
   pthread_t* parentThread;
   
   int rc;
   rc = pthread_create(parentThread, NULL, parent, (void*)0);
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
   }
   
   pthread_exit(NULL);
   
	return 0;
}

void *parent(void* pid)
{
   int PID = (int)pid;
   pthread_t children[NUM_CHILDREN];
   
   if (access(FIFO_NAME, F_OK) == -1)
   {
      int errno = mkfifo(FIFO_NAME, 0777);
      
      if(errno == -1)
      {
         printf("could not create FIFO. error %d\n", errno);
         exit(1);
      }
   }
   
   pipeFD = open(FIFO_NAME, O_RDWR);
   
   write(pipeFD, &gCounter, sizeof(gCounter));
   
   printf("Parent\n");
   
   int rc;
   for (long i = 1; i <= NUM_CHILDREN; ++i)
   {
      rc = pthread_create(children + (i-1), NULL, child, (void*) i);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   
   for (int i = 0; i < NUM_CHILDREN; i++)
      pthread_join(children[i], NULL);
   
   close(pipeFD);
   
   pthread_exit(NULL);
}

void *child(void* pid)
{
   int PID = (int)pid;
   
   printf("child\n");
   
   int readNum;
   
   while (readNum < 100)
   {
      sem_wait(&counter);
      //printf("reading\n");
      read(pipeFD, &readNum, sizeof(readNum));
      if (readNum < 100)
      {
         readNum++;
         printf("Counter: %d, PID: %d\n", readNum,PID);
      }
      gCounter = readNum;
      write(pipeFD, &readNum, sizeof(readNum));
      sem_post(&counter);
   }
   
   pthread_exit(NULL);
}
