#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 3

void* print_thread_message(void* arg) {
    int thread_num = *(int*)arg;

    while (1) {

	printf("I am thread %d\n", thread_num);
        fflush(stdout);  

    }
}

int main() {

    pthread_t threads[N];
    int thread_nums[N];
    for(int i = 0; i < N; i++)
      thread_nums[i] = i;
    
    
    // Create N threads
    for (int i = 0; i < N; i++)
      pthread_create(&threads[i], NULL, print_thread_message, &thread_nums[i]);
    
    // Wait for all threads (though in this case, the threads run forever)
    for (int i = 0; i < N; i++)
      pthread_join(threads[i], NULL);

    return 0;
}
