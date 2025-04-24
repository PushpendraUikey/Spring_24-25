#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BATCH 5
sem_t mutex;
sem_t cv_prod;
sem_t cv_req;
volatile int cnt = 0;

void * request(void *arg){
    sem_wait(&mutex);   // down
    cnt++;
    if(cnt == BATCH){
        sem_post(&cv_prod); // last one will wake up the producer
    }
    sem_post(&mutex);   // up
    sem_wait(&cv_req);  // request thread's will be blocked here
    printf("Request %d has been processed\n", *(int*)arg);
    return NULL;
}

void * producer(void* arg){
    sem_wait(&cv_prod);
    // when it'll be unblocked/waken up threre'd be 'BATCH' request pending
    for(int i=0; i<BATCH; i++){
        sem_post(&cv_req);   // wake up all waiting on the cv_req semaphore
    }
    return NULL;
}

/* ############%%%%%%%%%% Alternative to the above implementation %%%%%%%%%%%###########*/
// void * request(void *arg){
//     sem_wait(&mutex);   // down
//     cnt++;
//     if(cnt == BATCH){
//         sem_post(&cv_prod);
//     }
//     sem_post(&mutex);
//     sem_wait(&cv_req);  // request thread's will be blocked here
//     printf("Request %d has been processed\n", *(int*)arg);
//     sem_post(&cv_req);  // wakeup subsequent waiting requert threads 
//     return NULL;
// }

// void * producer(void* arg){
//     sem_wait(&cv_prod);
//     // when it'll be unblocked/waken up threre'd be 'BATCH' request pending
//     sem_post(&cv_req);  // wakeup only one.
//     return NULL;
// }

int main(int argc, char* argv[]){
    /// Params: first: semaphore, second: 0 indicates thread sharing btw same process, third: initial value of semaphore.
    sem_init(&mutex, 0, 1);
    sem_init(&cv_prod, 0, 0);
    sem_init(&cv_req, 0, 0);
    pthread_t p[BATCH];
    int val[BATCH];
    int i;
    pthread_t prod;
    pthread_create(&prod, NULL, producer, NULL);
    for(i=0; i<BATCH; i++){
        val[i] = i;
        pthread_create(&p[i], NULL, request, (void*)&val[i]);
    }
    pthread_join(prod, NULL);
    for(i=0; i<BATCH; i++){
        pthread_join(p[i], NULL);   // don't care about return;
    }

    // clean up
    sem_destroy(&mutex);
    sem_destroy(&cv_prod);
    sem_destroy(&cv_req);
}