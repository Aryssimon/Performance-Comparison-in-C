#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/ts_semaphore.h"
#include "../headers/test_and_set.h"


semaphore* ts_sem;


int ts_sem_init(semaphore **sem, unsigned int value) {
  *sem = (semaphore*) malloc(sizeof(semaphore));
  if (sem == NULL) fprintf(stderr, "malloc failed\n");
  (*sem)->val = value;
  lock_init_ts(&((*sem)->mutex));
  lock_init_ts(&((*sem)->wait_mutex));
  lock_ts((*sem)->wait_mutex);
  return 0;
}

int ts_sem_destroy(semaphore **sem) {
  lock_destroy_ts(&((*sem)->mutex));
  lock_destroy_ts(&((*sem)->wait_mutex));
  free(*sem);
  return 0;
}

int semaphore_post_ts(semaphore* sem){
  lock_ts(sem->mutex);
  sem->val = sem->val + 1;
  const int value = sem->val;
  unlock_ts(sem->mutex);

  if (value <= 0) {
    unlock_ts(sem->wait_mutex);
  }
  return 0;

}

int semaphore_wait_ts(semaphore* sem){
  lock_ts(sem->mutex);
  sem->val = sem->val - 1;
  const int value = sem->val;
  unlock_ts(sem->mutex);

  if (value < 0) {
    lock_ts(sem->wait_mutex);
  }
  return 0;
}
