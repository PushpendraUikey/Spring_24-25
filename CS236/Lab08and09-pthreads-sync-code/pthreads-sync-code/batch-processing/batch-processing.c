#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Wrong code and logic as well.    

#define MAXBATCH 5

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_req = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
volatile int count = 1;
volatile int consumed = 0;
volatile int batch_started = 0;
int req_;

void * request(void *arg){
    pthread_mutex_lock(&lock);

    while(count <= req_){
        if(!batch_started)
            count++;
        if(!(count%MAXBATCH)){
            pthread_cond_signal(&cond_prod);    // singal batch is ready!
        }

        if(count > req_) break; // work finished for every request thread

        while(!batch_started){
            pthread_cond_wait(&cond_req, &lock);
        }
        
        consumed++;
        printf("%d request finished by thread %d\n", consumed, *(int*)arg);
        if(consumed==count) batch_started = 0;
    }
    pthread_cond_broadcast(&cond_req);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void * process(void * arg){
    pthread_mutex_lock(&lock);
    while(consumed < req_){
        if(batch_started) 
            pthread_cond_wait(&cond_prod, &lock);

        while((count % MAXBATCH)){
            pthread_cond_wait(&cond_prod, &lock);
        }

        batch_started = 1;
        pthread_cond_broadcast(&cond_req);
    }
    
    pthread_cond_broadcast(&cond_req);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char* argv[]){
    pthread_t batch_p;
    int i;
    printf("number of requests: ");
    scanf("%d", &req_);
    if(req_ % MAXBATCH){
        printf("Request put the multiple of %d\n", MAXBATCH);
        exit(1);
    }
    
    pthread_t p[MAXBATCH];
    int val[MAXBATCH];

    pthread_create(&batch_p, NULL, process, NULL);
    for(i=0; i<MAXBATCH; i++){
        val[i] = i;
        pthread_create(&p[i], NULL, request, (void*)&val[i]);
    }

    pthread_join(batch_p, NULL);    // if threads aren't joined then main thread might get exit before the 
    // even the other threads join hence they won't even run.
    for(i=0; i<MAXBATCH; i++){
        pthread_join(p[i], NULL);
    }
    printf("Finished batch processing now on main function\n");
}