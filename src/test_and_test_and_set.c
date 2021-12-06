#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../headers/test_and_test_and_set.h"

int counter_tts = 0;
const int NB_SECTIONS_TTS = 6400;

int test_and_set_tts(int *locker){
  int value = 1;
  asm ("xchgl %0, %1"
  : "+q" (value), "+m"(*locker));
  return value;
}

int lock_init_tts(int **locker){
  *locker = (int*) malloc(sizeof(int));
  if (locker == NULL) fprintf(stderr, "malloc failed\n");
  *(*locker) = 0;
  return 0;
}

int lock_destroy_tts(int **locker) {
  free(*locker);
  return 0;
}

int lock_tts(int *locker){
  while(test_and_set_tts(locker)){
      while(*locker);
  }
  return 0;
}

int unlock_tts(int *locker){
  asm ("movl $0, %%eax\n"
      "xchgl %%eax, %0"
  : "+m"(*locker)
  :
  : "eax");
  return 0;
}


void* thread_work_tts(void* arg){
  int *mutex = (int *) arg;
  while(1) {
    lock_tts(mutex);
    if (counter_tts == NB_SECTIONS_TTS) {
      unlock_tts(mutex);
      break;
    }
    while(rand() > RAND_MAX/10000);
    counter_tts++;
    unlock_tts(mutex);
  }
  return NULL;
}
