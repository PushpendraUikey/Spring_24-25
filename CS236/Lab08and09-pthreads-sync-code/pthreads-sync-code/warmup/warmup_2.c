#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


void* threadName(void* arg){
    int* val = (int *)arg;
    printf("I'm the thread %d\n", *val);
    return NULL;
}


int main(int argc, char* argv[]){
    int diff=0;
    const int N = 15;

    int * val = (int*)malloc(N*sizeof(int));

    pthread_t p[N];
    for(int i=0; i<N; i++){
        val[i] = i;
        pthread_create(&p[i], NULL, threadName, &val[i]);
    }
    for(int j=0; j<N; j++){
        pthread_join(p[j], NULL);
    }

    printf("I'm the parent thread\n");
    free(val);
    return 0;
}