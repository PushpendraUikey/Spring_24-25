#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->lock = PTHREAD_MUTEX_INITIALIZER;
  rw->commonlock = PTHREAD_MUTEX_INITIALIZER;
  rw->cond_reader = PTHREAD_COND_INITIALIZER;
  rw->cond_writer = PTHREAD_COND_INITIALIZER;
  rw->numreader = 0;
  rw->numwriter = 0;
  rw->read_avail = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->commonlock);
  while(rw->numwriter){   // if writer's are waiting then don't allow reader to proceed.
    pthread_cond_wait(&rw->cond_reader, &rw->commonlock);
  }
  rw->numreader++;  // when no writer there new bundle/batch of readers who can read simultaneously
  // if(rw->read_avail){
  //   pthread_mutex_unlock(&rw->commonlock);
  //   return;
  // }

  if(rw->numreader == 1) 
    pthread_mutex_lock(&rw->lock);
  pthread_mutex_unlock(&rw->commonlock);
  // pthread_mutex_lock(&rw->lock);
  // rw->read_avail = 1; // first reader will make it true in current batch[when no writer is there]
  
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->commonlock);
  rw->numreader--;
  if(rw->numreader > 0){  // still readers remaining out there in current batch
    pthread_mutex_unlock(&rw->commonlock);
    return;
  }

  // rw->read_avail = 0;
  // pthread_cond_broadcast(&rw->cond_writer); // wakeup waiting writer threads
  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_unlock(&rw->lock);    // final unlock(last reading process will unlock it)

}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->commonlock);
  rw->numwriter++;
  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_lock(&rw->lock);
  return;
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->commonlock);
  rw->numwriter--;
  if(!rw->numwriter){
    pthread_cond_broadcast(&rw->cond_reader); // wakeup all reader waiting processes since now no writer is there
  }
  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_unlock(&rw->lock);
}


//////////////// ####### INITIAL WRONG IMPLEMENTATION HERE ######### ////////////////

// ############## ####### Reader Lock ###### ################
  // if(!rw->reader){
  //   if(!rw->lockheld && !rw->numwriter){
  //     pthread_mutex_lock(&rw->lock);
  //     rw->numreader++;
  //     rw->lockheld = 1;
  //   }else{
  //     if(!rw->numwriter){   // If no writer is waiting or writing
  //       rw->numreader++;
  //       rw->lockheld = 1;
  //     }else{
  //       pthread_mutex_lock(&rw->lock);  // it'll blocked here

  //       while(rw->numwriter){   // Wait till no writer is there 
  //         pthread_cond_wait(&rw->cond, &rw->lock);
  //       }

  //       rw->numreader++;
  //       rw->lockheld = 1;
  //     }
      
  //   }
  // }else{
  //   perror("Wrong Lock Acquire\n");
  //   exit(0);
  // }

// ############## ####### Reader Unlock ###### ################
  // if(!rw->reader){

  //   if(!rw->numreader){
  //     perror("Wrong Unlock\n");
  //     exit(1);
  //   }
  //   if(!rw->numwriter){
  //     rw->numreader--;
  //   }else{
  //     pthread_cond_broadcast(&rw->cond);
  //     pthread_mutex_unlock(&rw->lock);
  //   }
    
  //   if(!rw->numreader && !rw->numwriter) rw->lockheld = 0;
  // }else{
  //   perror("Wrong Unlock\n");
  //   exit(1);
  // }

// ############## ####### Writer Lock ###### ################
  // if(!rw->reader){
  //   if(!rw->lockheld){
  //     pthread_mutex_lock(&rw->lock);
  //     rw->numwriter++;
  //     rw->lockheld = 1;
  //   }else{
  //     rw->numwriter++;
  //     pthread_mutex_lock(&rw->lock);
  //     rw->lockheld = 1;
  //     // if(!rw->numwriter){ // no reader had already acquired the lock
  //     //   pthread_mutex_lock(&rw->lock);  // it'll blocked here if writer reader has lock
  //     //   rw->numwriter++;
  //     //   rw->lockheld = 1;
  //     // }else{
  //     //   rw->numwriter++;
  //     //   rw->lockheld = 1;
  //     // }
  //   }
  // }else{
  //   perror("Wrong Lock Acquire\n");
  //   exit(0);
  // }

// ############# ###### Writer Unlock ###### ################
// if(!rw->reader){
  //   if(!rw->lockheld){
  //     perror("lock error!\n");
  //     exit(1);
  //   }
  //   if(!rw->numwriter){
  //     perror("Wrong Unlock\n");
  //     exit(1);
  //   }else{
  //     rw->numwriter--;
  //     rw->lockheld = 0;
  //     pthread_cond_broadcast(&rw->cond);
  //     pthread_mutex_unlock(&rw->lock);
  //   }
  //   // if(rw->numwriter > 1){
  //   //   rw->numwriter--;
  //   // }else if(rw->numwriter == 1){
  //   //   rw->numwriter--;
  //   //   rw->lockheld = 0;
  //   //   pthread_cond_broadcast(&rw->cond);
  //   //   pthread_mutex_unlock(&rw->lock);
  //   // }else{
  //   //   perror("Wrong Unlock\n");
  //   //   exit(0);
  //   // }
  // }else{
  //   perror("Wrong Unlock\n");
  //   exit(0);
  // }