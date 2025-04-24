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
  rw->numreader++;
  if(rw->numreader > 1){
    if(!rw->read_avail){
      pthread_cond_wait(&rw->cond_reader, &rw->commonlock);
    }
    pthread_mutex_unlock(&rw->commonlock);
    return;
  }
  
  pthread_cond_broadcast(&rw->cond_reader); // wake up all the readers waiting
  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_lock(&rw->lock);
  rw->read_avail = 1;

}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->commonlock);
  rw->numreader--;
  if(rw->numreader > 0){  // still readers remaining out there
    pthread_mutex_unlock(&rw->commonlock);
    return;
  }

  rw->read_avail = 0;

  pthread_cond_broadcast(&rw->cond_writer); // wakeup waiting writer threads
  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_unlock(&rw->lock);  // final unlock

}

void WriterLock(struct read_write_lock * rw)
{
  pthread_mutex_lock(&rw->commonlock);

  while(rw->numreader){ // since reader has to be given preference over writer
    pthread_cond_wait(&rw->cond_writer, &rw->commonlock);
  }

  pthread_mutex_unlock(&rw->commonlock);
  pthread_mutex_lock(&rw->lock);

  return;
  
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_unlock(&rw->lock);  // It'll just release the lock no other job.

}

// ############## ###### Read Lock ######## ###############
  // if(rw->reader){
  //   if(!rw->lockheld){
  //     pthread_mutex_lock(&rw->lock);
  //     rw->numreader++;
  //     rw->lockheld = 1;
  //   }else{
  //     if(!rw->numreader){ // no reader had already acquired the lock
  //       pthread_mutex_lock(&rw->lock);  // it'll blocked here
  //       rw->numreader++;
  //       rw->lockheld = 1;
  //     }else{
  //       rw->numreader++;
  //       rw->lockheld = 1;
  //     }
  //   }
  // }else{
  //   perror("Wrong Lock Acquire\n");
  //   exit(0);
  // }

// ############## ###### Read Unlock ####### ###############
  // if(rw->reader){
  //   if(!rw->lockheld){
  //     perror("lock error!\n");
  //     exit(1);
  //   }
  //   if(rw->numreader > 1){
  //     rw->numreader--;
  //   }else if(rw->numreader == 1){
  //     rw->numreader--;
  //     rw->lockheld = 0;
  //     pthread_cond_broadcast(&rw->cond);
  //     pthread_mutex_unlock(&rw->lock);
  //   }else{
  //     perror("Wrong Unlock\n");
  //     exit(0);
  //   }
  // }else{
  //   perror("Wrong Unlock\n");
  //   exit(0);
  // }

// ############## ###### write lock ####### ###############
  //	Write the code for aquiring read-write lock by the writer.
  // if(rw->reader){
  //   if(!rw->lockheld){
  //     pthread_mutex_lock(&rw->lock);
  //     rw->numwriter++;
  //     rw->lockheld = 1;
  //   }else{
  //     pthread_mutex_lock(&rw->lock);

  //     while(rw->numreader){   // Wait till no reader is there 
  //       pthread_cond_wait(&rw->cond, &rw->lock);
  //     }

  //     rw->numwriter++;
  //     rw->lockheld;
  //   }
  // }else{
  //   perror("Wrong Writer Lock\n");
  //   exit(1);
  // }


// ############## ###### Write Unlock ###### ###############
  // if(rw->reader){

  //   if(!rw->numwriter){
  //     perror("Wrong Unlock\n");
  //     exit(1);
  //   }
  //   rw->numwriter--;
  //   rw->lockheld = 0;
  //   pthread_cond_broadcast(&rw->cond);
  //   pthread_mutex_unlock(&rw->lock);
  // }else{
  //   perror("Wrong Unlock\n");
  //   exit(1);
  // }