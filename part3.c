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
    
    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;
    
    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    printf("created segment in memory\n");
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == -1) {
        perror("shmat");
        exit(1);
    }
    printf("Attached segment to data space\n");
    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    //s = shm;
    
    //for (c = 0; c <= 100; c++)
        //s = c;
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
    printf("init all children\n");
    /*
     * Finally, we wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*shm != 100)
        sleep(1);
    printf("s is 100\n");
    pthread_exit(NULL);
}

void *child(void* pid)
{
    int PID = (int)pid;
    
    printf("child\n");
    
    int shmid;
    key_t key;
    int *shm, *s;
    
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;
    
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHM_SIZE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    /*
     * Now read what the server put in the memory.
     */
     printf("attached memory for sharing\n");
    for (s = shm; *s != 100; *s++)
    {
        printf("s: \n", *s);
        sem_wait(&counter);
        printf("Counter: %d, PID: %d\n", *s,PID);
        sem_post(&counter);
    }
    //putchar('\n');
    printf("children got to 100\n");
    /*
     * Finally, change the first character of the
     * segment to '*', indicating we have read
     * the segment.
     */
    // *shm = '*';
    
    /*
    while (s != 100)
    {
        sem_wait(&counter);
        s++;
        printf("Counter: %d, PID: %d\n", s,PID);
        sem_post(&counter);
    }*/
    
    pthread_exit(NULL);
}
