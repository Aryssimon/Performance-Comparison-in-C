#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/our_semaphore.h"
#include "../headers/test_and_test_and_set.h"


semaphore* our_sem;


int our_sem_init(semaphore **sem, unsigned int value) {
  *sem = (semaphore*) malloc(sizeof(semaphore));
  if (sem == NULL) fprintf(stderr, "malloc failed\n");
  (*sem)->val = value;
  lock_init(&((*sem)->mutex));
  lock_init(&((*sem)->wait_mutex));
  lock_tts((*sem)->wait_mutex);
  return 0;
}

int our_sem_destroy(semaphore **sem) {
  lock_destroy(&((*sem)->mutex));
  lock_destroy(&((*sem)->wait_mutex));
  free(*sem);
  return 0;
}

int semaphore_post(semaphore* sem){
  lock_tts(sem->mutex);
  sem->val = sem->val + 1;
  const int value = sem->val;
  unlock_tts(sem->mutex);

  if (value <= 0) {
    unlock_tts(sem->wait_mutex);
  }
  return 0;

}

int semaphore_wait(semaphore* sem){
  lock_tts(sem->mutex);
  sem->val = sem->val - 1;
  const int value = sem->val;
  unlock_tts(sem->mutex);

  if (value < 0) {
    lock_tts(sem->wait_mutex);
  }
  return 0;
}
