/*
 Authors: Gabrielle Kenney
          Eric Nicholson
 
 Description:
   This program simulates process syncronization between a parent and multiple children proccesses.
   The processes are simulated by using threads. Each 'child' thread will increment a global variable
   by 1 until that value reaches 100.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t counter;



int gCounter = 0;

#define NUM_CHILDREN 5


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
   
   pthread_exit(NULL);
}

void *child(void* pid)
{
   int PID = (int)pid;
   
   printf("child\n");
   
   while (gCounter < 100)
   {
      sem_wait(&counter);
      gCounter++;
      printf("Counter: %d, PID: %d\n", gCounter,PID);
      sem_post(&counter);
   }
   
   pthread_exit(NULL);
}
