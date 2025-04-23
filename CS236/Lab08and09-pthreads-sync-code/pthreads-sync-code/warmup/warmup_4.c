#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// typedef struct{
//     int threadnum;
//     pthread_mutex_t *lock;
//     pthread_cond_t *cond;
// } conditionvars;
const int N = 10;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int readythread = 0;

void* threadName(void* arg){
    // conditionvars* value = (conditionvars *)arg;
    // int randomnumber;
    // randomnumber = rand() % 5;
    // sleep(randomnumber);
    int threadnum = * (int *)arg;
    pthread_mutex_lock(&lock);
    while (readythread != threadnum){
        // printf("thread before wait%d\n", threadnum);
        pthread_cond_wait(&cond, &lock);
        // printf("thread %d has woke up\n", threadnum);
    }
    printf("thread: %d\n", threadnum);
    // printf("I'm thread %d\n", threadnum);
    readythread++;
    // printf("broadcasted\n");
    pthread_cond_broadcast(&cond);  // signal another thread.
    pthread_mutex_unlock(&lock);
    return NULL;
}


int main(int argc, char* argv[]){

    // conditionvars * values = (conditionvars*)malloc(N*sizeof(conditionvars));
    // pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    // pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    int * val = (int*)malloc(N*sizeof(int));
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
    free(val);
    return 0;
}