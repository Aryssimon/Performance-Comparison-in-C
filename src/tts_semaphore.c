#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/tts_semaphore.h"
#include "../headers/test_and_test_and_set.h"


semaphore* tts_sem;


int tts_sem_init(semaphore **sem, unsigned int value) {
  *sem = (semaphore*) malloc(sizeof(semaphore));
  if (sem == NULL) fprintf(stderr, "malloc failed\n");
  (*sem)->val = value;
  lock_init_tts(&((*sem)->mutex));
  lock_init_tts(&((*sem)->wait_mutex));
  lock_tts((*sem)->wait_mutex);
  return 0;
}

int tts_sem_destroy(semaphore **sem) {
  lock_destroy_tts(&((*sem)->mutex));
  lock_destroy_tts(&((*sem)->wait_mutex));
  free(*sem);
  return 0;
}

int semaphore_post_tts(semaphore* sem){
  lock_tts(sem->mutex);
  sem->val = sem->val + 1;
  const int value = sem->val;
  unlock_tts(sem->mutex);

  if (value <= 0) {
    unlock_tts(sem->wait_mutex);
  }
  return 0;

}

int semaphore_wait_tts(semaphore* sem){
  lock_tts(sem->mutex);
  sem->val = sem->val - 1;
  const int value = sem->val;
  unlock_tts(sem->mutex);

  if (value < 0) {
    lock_tts(sem->wait_mutex);
  }
  return 0;
}
