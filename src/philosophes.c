#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/philosophes.h"
#include "../headers/test_and_test_and_set.h"

const int CYCLES = 100000;

void* philosophe(void* args) {
  phil_args *arguments = (phil_args *) args;
  pthread_mutex_t *baguette = arguments->baguette;
  int left = arguments->index;
  int right = (left + 1) % arguments->N_BAGUETTES;
  for(int i = 0; i < CYCLES; i++) {
    if(left<right) {
      pthread_mutex_lock(&baguette[left]);
      pthread_mutex_lock(&baguette[right]);
    }
    else {
      pthread_mutex_lock(&baguette[right]);
      pthread_mutex_lock(&baguette[left]);
    }
    pthread_mutex_unlock(&baguette[left]);
    pthread_mutex_unlock(&baguette[right]);
  }
  return NULL;
}

void* our_philosophe(void* args) {
  our_phil_args *arguments = (our_phil_args *) args;
  int **baguette = arguments->baguette;
  int left = arguments->index;
  int right = (left + 1) % arguments->N_BAGUETTES;
  for(int i = 0; i < CYCLES; i++) {
    if(left<right) {
      lock_tts(baguette[left]);
      lock_tts(baguette[right]);
    }
    else {
      lock_tts(baguette[right]);
      lock_tts(baguette[left]);
    }
    unlock_tts(baguette[left]);
    unlock_tts(baguette[right]);
  }
  return NULL;
}
