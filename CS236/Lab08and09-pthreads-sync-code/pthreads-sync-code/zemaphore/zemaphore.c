#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    s->value = value;
    pthread_mutex_init(&(s->lock), NULL);   // lock initialization
    pthread_cond_init(&(s->cond), NULL);    // condition variable initialization
}

// This is to acquire the zemaphore!
void zem_down(zem_t *s) {
    pthread_mutex_lock(&(s->lock));
    if(--(s->value) < 0 ){
        pthread_cond_wait(&(s->cond), &(s->lock));
    }
    pthread_mutex_unlock(&(s->lock));
}

// This is to wake-up a zemaphore equivalent to waking up a sleeping zemaphore!
void zem_up(zem_t *s) {
    pthread_mutex_lock(&(s->lock));
    s->value++;
    pthread_cond_signal(&(s->cond));    // wake up one waiting process
    pthread_mutex_unlock(&(s->lock));
}
