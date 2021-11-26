#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/philosophes.h"

const int CYCLES = 100000;

void* philosophe(void* args) {
  phil_args *arguments = (phil_args *) args;
  pthread_mutex_t *baguette = arguments->baguette;
  int left = arguments->index;
  int right = (left + 1) % arguments->N_BAGUETTES;
  for(int i = 0; i < CYCLES; i++) {
    // philosophe pense
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
  pthread_exit (NULL);
}
