#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/philosophes.h"
#include "../headers/test_and_test_and_set.h"

const int CYCLES = 5;

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

void* our_philosophe(void* args) {
  int personal_counter = 0;
  our_phil_args *arguments = (our_phil_args *) args;
  int **baguette = arguments->baguette;
  int left = arguments->index;
  int right = (left + 1) % arguments->N_BAGUETTES;
  for(int i = 0; i < CYCLES; i++) {
    // philosophe pense
    if(left<right) {
      lock_tts(baguette[left]);
      printf("philosophe %d prend la baguette %d\n",left,left);
      lock_tts(baguette[right]);
      printf("philosophe %d prend la baguette %d\n",left,right);
      printf("philosophe %d mange\n",left);
      personal_counter++;
    }
    else {
      lock_tts(baguette[right]);
      printf("philosophe %d prend la baguette %d\n",left,right);
      lock_tts(baguette[left]);
      printf("philosophe %d prend la baguette %d\n",left,left);
      printf("philosophe %d mange\n",left);
      personal_counter++;
    }
    unlock_tts(baguette[left]);
    printf("philosophe %d relache la baguette %d\n",left,left);
    unlock_tts(baguette[right]);
    printf("philosophe %d relache la baguette %d\n",left,right);
    printf("philosophe %d pense\n",left);
  }
  printf("personal counter of %d : %d\n",left,personal_counter);
  pthread_exit (NULL);
}
