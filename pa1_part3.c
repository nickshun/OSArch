#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>


sem_t counter;

int gCounter = 0;

#define NUM_CHILDREN 5
#define SHM_SIZE     27

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
    
    int c;
    int shmid;
    key_t key;
    int *shm, *s;
    
   
    key = 5678;
    
   
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
   
   shm = shmat(shmid, NULL, 0);
    if (*shm == -1) {
        perror("shmat");
        exit(1);
    }
   
    s = shm;
    *s = 0;
    printf("set s to 0\n");
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
   
    pthread_exit(NULL);
}

void *child(void* pid)
{
    int PID = (int)pid;
    
    printf("child\n");
    
    int shmid;
    key_t key;
    int *shm, *s;
   
    key = 5678;
   
    if ((shmid = shmget(key, SHM_SIZE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    shm = shmat(shmid, NULL, 0);
    if (*shm == -1) {
        perror("shmat");
        exit(1);
    }
    for (s = shm; *s <= 100; (*s) += 1)
    {
        sem_wait(&counter);
        printf("Counter: %d, PID: %d\n", *s,PID);
        sem_post(&counter);
    }
    
    pthread_exit(NULL);
}
