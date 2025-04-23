#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// typedef struct{
//     int threadnum;
//     pthread_mutex_t *lock;
//     pthread_cond_t *cond;
// } conditionvars;

/*
* when condition_broadcast is called:
* All threads waiting on the condition variable are moved from the 
* condition queue to the mutex queue (they try to reacquire the associated mutex).
*
* if mutex locks are blocked due to attempt to acquire (pushed into mutex queue) then 
* whenever one unlocks this lock OS implementation would simply wake up one thread according 
* to its policy.
* The above doesn't happens for the threads waiting on conditional variables: somebody has 
* to wake them up.
*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int readythread = 0;

void* threadName(void* arg){
    // conditionvars* value = (conditionvars *)arg;
    int randomnumber;
    randomnumber = rand() % 5;
    sleep(randomnumber);
    int threadnum = * (int *)arg;
    pthread_mutex_lock(&lock);
    while (readythread != threadnum){
        // printf("thread before wait%d\n", threadnum);
        pthread_cond_wait(&cond, &lock);
        // printf("thread %d has woke up\n", threadnum);
    }

    printf("I'm thread %d\n", threadnum);
    readythread++;
    // printf("broadcasted\n");
    pthread_cond_broadcast(&cond);  // signal another thread.
    pthread_mutex_unlock(&lock);
    return NULL;
}


int main(int argc, char* argv[]){
    const int N = 10;

    // conditionvars * values = (conditionvars*)malloc(N*sizeof(conditionvars));
    // pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    // pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    // int * val = (int*)malloc(N*sizeof(int));
    int val[N]; // not necessary to have global variable or variabl in heap: bcz any way we're passing the
                // address of var to thread and just have to make sure the content don't change
    pthread_t p[N];
    for(int i=0; i<N; i++){
        // values[i] = {i, lock, cond};
        val[i]=i;
        pthread_create(&p[i], NULL, threadName, &val[i]);
    }
    for(int j=0; j<N; j++){
        pthread_join(p[j], NULL);
    }

    printf("I'm the parent thread\n");
    // free(val);
    return 0;
}

/*
Before wake-up: Threads are in the condition variable's wait queue (managed by pthreads).

After pthread_cond_broadcast:

All are moved to the ready queue.

Only one gets the mutex and runs.

Others are ready-to-run, waiting in the OS ready queue (typically implemented as a priority queue 
or linked list depending on the OS scheduler).

They no longer wait on the condition variable unless explicitly re-waited by calling pthread_cond_wait again.
*/