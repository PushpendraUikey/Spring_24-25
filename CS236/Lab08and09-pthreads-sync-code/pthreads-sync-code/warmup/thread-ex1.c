#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    int a;
    int b;
} myargs_t;
typedef struct{ int x;  int y; } myret_t;

int val = 100;

void * mythread(void* arg){ // Any kind of arguments can be passed to the function: same as to the thread_join
    // myargs_t * args = (myargs_t *) arg; // Do type casting;
    // printf("%d %d \n", args->a, args->b);
    // val = 1;
    // return NULL;
    myret_t *rvals = malloc(sizeof(myret_t));
    rvals -> x = 78;
    rvals -> y = 23487;
    return (void *)rvals;
}

void * mythread_2(void* arg){
    long long int value = (long long int) arg;  // little confusing type-cast
    printf("value inside the thread %lld\n", value);
    return (void *) (value+1);
}

int main(int argc, char* argv[]){
    pthread_t p;
    // myret_t *rvals;
    long long int rvalue = 89813878912;
    // printf("%d\n", val);
    // myargs_t arg = {10, 20};

    // Arguments: (pointer to thread, attributes like scheduler priority, function like: ret_type (*<funct-name>)(arg_type), arguments)
    int rc = pthread_create(&p, NULL, mythread_2, (void*)rvalue);  // function name is a pointer and all the other arguments have to be pointer.
    // pthread_join(p, (void**)&rvals);        // This stops the main thread to wait for the thread "p" to complete execution.
    pthread_join(p, (void**)&rvalue);
    printf("returned value: %lld\n", rvalue);
    // printf("returned values from the thread are x=%d and y=%d\n", rvals->x, rvals->y);
    // free(rvals);    // Dynamically allocated memory initaaly
    // sleep(2);
    // printf("%d\n", val);
    return 0;
}