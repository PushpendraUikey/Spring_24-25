#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int counter = 0;

void * incrementcounter(void* arg){
    for(int i=0; i<1000; i++){
        counter++;
    }
}
int main(int argc, char* argv[]){
    int diff=0;
    for(int k=0; k<100; k++){
        pthread_t p[10];
        for(int i=0; i<10; i++){
            pthread_create(&p[i], NULL, incrementcounter, NULL);
        }
        for(int j=0; j<10; j++){
            pthread_join(p[j], NULL);
        }
        printf("counter: %d\n", counter);
        diff = (counter == 10000) ? diff : diff + 1;
        counter = 0;
    }
    printf("Different output got out of 100: %d\n", diff);
    return 0;
}