#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

// Global variables declarations to implement master-worker
int item_to_produce;
int item_to_read;
int curr_buf_size; 
int curr_cons_size;
int num_workers;
int num_masters;
int total_items;
int max_buf_size;
int ready=1;

// initializations for threads, cond_prod: producers to wait, cond_cons: consumers to wait
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

// memory holding values/requests
int *buffer;

void print_produced(int num, int master) {
  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {
  printf("Consumed %d by worker %d\n", num, worker);
}

//produce items and place in buffer
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  // @imp: using lock here isn't the purpose of this exercise, it makes single thread
  // producing many items in the buffer.
  pthread_mutex_lock(&lock);
  while(ready)
    {
      // job finished
      if(item_to_produce >= total_items) {
	      break;
      }
      // if buffer is full: wait on cond_prod CV queue
      while(((curr_buf_size+1)%max_buf_size) == curr_cons_size)
        {
          pthread_cond_wait(&cond_prod, &lock);   // wait on producer thread until woken up by someone
          if(!ready){
            break;
          }
        }

      buffer[curr_buf_size++] = item_to_produce;
      curr_buf_size %= max_buf_size;
      print_produced(item_to_produce, thread_id);
      item_to_produce++;
      // important as someone waiting for lock to be freed won't know if data is available or not
      // It is also practiced against deadlock: produced something wakeup one consumer to consume it.
      pthread_cond_signal(&cond_cons);   // wakeup some consumer thread to consume it as data available
    }

  ready = 0;
  pthread_cond_broadcast(&cond_prod); // wake up all the producer to stop them
  pthread_mutex_unlock(&lock);
  return 0;
}

void * consume_request_loop(void * arg){
  int thread_id = *((int *) arg);
  int val;

  // this lock needs to be placed inside the while()
  pthread_mutex_lock(&lock);
  while(item_to_read < total_items)
    {
      // if the buffer is empty!
      while(curr_buf_size == curr_cons_size) {
        if(item_to_read >= total_items) { // somebody else has finished the job
          break;
        }
        pthread_cond_wait(&cond_cons, &lock); // Wait on consumer queue
      }
      if(item_to_read >= total_items) {
        break;
      }

      val = buffer[curr_cons_size++];
      curr_cons_size %= max_buf_size;
      print_consumed(val, thread_id);
      item_to_read++;

      pthread_cond_signal(&cond_prod);   // wakeup some-one to write if waiting since empty place found
    }
    ready = 0;
    pthread_cond_broadcast(&cond_cons); // Wakeup all the consumers to stop them
    pthread_mutex_unlock(&lock);
  return 0;
}

int main(int argc, char *argv[])
{
  int *master_thread_id, *worker_thread_id;
  pthread_t *master_thread, *worker_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  curr_cons_size = 0;
  item_to_read = 0;
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

   buffer = (int *)malloc (sizeof(int) * max_buf_size);

   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
   worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;

  for (i = 0; i < num_workers; i++)
    worker_thread_id[i] = i;

  for (i = 0; i < num_masters; i++)
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
  for (i = 0; i < num_workers; i++)
    pthread_create(&worker_thread[i], NULL, consume_request_loop, (void *)&worker_thread_id[i]);


  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }
  
  for (i = 0; i < num_workers; i++)
    {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", worker_thread_id[i]);
    }
  
  // ----Deallocating Buffers---------------------
  free(buffer);
  free(master_thread_id);
  free(worker_thread_id);
  free(master_thread);
  free(worker_thread);

  return 0;
}


// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <semaphore.h>

// int item_prod;
// int item_cons;
// int item_to_produce;
// int total_items, max_buf_size;
// int num_workers, num_masters;
// int ready=1;
// sem_t lock;
// sem_t cond_prod;
// sem_t cond_cons;

// int *buffer;

// void print_produced(int num, int master) {
//     printf("Produced %d by master %d\n", num, master);
// }

// void print_consumed(int num, int worker) {
//     printf("Consumed %d by worker %d\n", num, worker);
// }

// //produce items and place in buffer
// void *generate_requests_loop(void *data)
// {
//     int thread_id = *((int *)data);

//     while(item_prod < total_items)
//         {
//             sem_wait(&cond_prod);
//             sem_wait(&lock);

//             if(item_prod >= total_items) {
//                 break;
//             }
        
//             buffer[(item_prod++ % max_buf_size)] = item_to_produce;
//             print_produced(item_to_produce, thread_id);
//             item_to_produce++;

//             sem_post(&lock);    // unlock it

//             sem_post(&cond_cons);   // wake up waiting consumer
//         }
//         sem_post(&lock);
//         sem_post(&cond_prod); // wake up all producer, job has finished
//     return 0;
// }
  
// void * consume_request_loop(void * arg){
//     int thread_id = *((int *) arg);
//     int val;

//     while(item_cons < total_items)
//         {
//             sem_wait(&cond_cons); 
            
//             sem_wait(&lock);    // lock fur mututal exclusion
//             if(item_cons >= total_items) {
//                 break;
//             }
//             val = buffer[(item_cons++ % max_buf_size)];
//             print_consumed(val, thread_id);
//             sem_post(&lock);    // wake up someone

//             sem_post(&cond_prod);   // wakeup producer that place is now empty.
//         }
//         sem_post(&lock);    // wake up all job finished!
//         sem_post(&cond_cons); // wake up all consumers job has finished.
//     return 0;
// }

// int main(int argc, char *argv[])
// {
//   int *master_thread_id, *worker_thread_id;
//   pthread_t *master_thread, *worker_thread;
//   item_prod = 0;
//   item_cons = 0;
//   item_to_produce = 0;
  
//   int i;
  
//    if (argc < 5) {
//     printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
//     exit(1);
//   }
//   else {
//     num_masters = atoi(argv[4]);
//     num_workers = atoi(argv[3]);
//     total_items = atoi(argv[1]);
//     max_buf_size = atoi(argv[2]);
//   }

//   sem_init(&lock, 0, 1);    // for lock implementation;
//   sem_init(&cond_cons, 0, 0);
//   sem_init(&cond_prod, 0, max_buf_size);    // number of empty slots in buffer

//    buffer = (int *)malloc (sizeof(int) * max_buf_size);

//    //create master producer threads
//    master_thread_id = (int *)malloc(sizeof(int) * num_masters);
//    worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
//    master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
//    worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
//   for (i = 0; i < num_masters; i++)
//     master_thread_id[i] = i;

//   for (i = 0; i < num_workers; i++)
//     worker_thread_id[i] = i;

//   for (i = 0; i < num_masters; i++)
//     pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
  
//   for (i = 0; i < num_workers; i++)
//     pthread_create(&worker_thread[i], NULL, consume_request_loop, (void *)&worker_thread_id[i]);


//   //wait for all threads to complete
//   for (i = 0; i < num_masters; i++)
//     {
//       pthread_join(master_thread[i], NULL);
//       printf("master %d joined\n", i);
//     }
  
//   for (i = 0; i < num_workers; i++)
//     {
//       pthread_join(worker_thread[i], NULL);
//       printf("worker %d joined\n", worker_thread_id[i]);
//     }
  
//   /*----Deallocating Buffers---------------------*/
//   free(buffer);
//   free(master_thread_id);
//   free(worker_thread_id);
//   free(master_thread);
//   free(worker_thread);

//   return 0;
// }