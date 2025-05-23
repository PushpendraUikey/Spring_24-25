#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include </home/pushpendra/Lab08-pthreads-sync-code/pthreads-sync-code/zemaphore/zemaphore.h>


#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zem[NUM_THREADS];

void *justprint(void *data)
{
  int thread_id = *((int *)data);

  for(int i=0; i < NUM_ITER; i++)
    { 
      zem_down(&(zem[thread_id]));
      printf("This is thread %d\n", thread_id);
      zem_up(&(zem[(thread_id+1)%NUM_THREADS]));
    }
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];
  
  for(int i=0; i<NUM_THREADS; i++){
    zem_init(&(zem[i]), 0);
  }

  zem_up(&zem[0]);  // wake up the first one, it'll do the later jobs
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}
