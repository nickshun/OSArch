#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int gCounter = 0;

#define NUM_CHILDREN 5

int main ()
{
   for (int i = 0; gCounter != 100; ++i)
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
   }
   
   return 0;
}
