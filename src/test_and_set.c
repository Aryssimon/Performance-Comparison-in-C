#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../headers/test_and_set.h"

int counter_ts = 0;
const int NB_SECTIONS_TS = 6400;

int test_and_set_ts(int *locker){
  int value = 1;
  asm ("xchgl %0, %1"
  : "+q" (value), "+m"(*locker));
  return value;
}

int lock_init_ts(int **locker){
  *locker = (int*) malloc(sizeof(int));
  if (locker == NULL) fprintf(stderr, "malloc failed\n");
  *(*locker) = 0;
  return 0;
}

int lock_destroy_ts(int **locker) {
  free(*locker);
  return 0;
}

int lock_ts(int *locker){
  while(test_and_set_ts(locker));
  return 0;
}

int unlock_ts(int *locker){
  asm ("movl $0, %%eax\n"
       "xchgl %%eax, %0"
  : "+m"(*locker)
  :
  : "eax");
  return 0;
}

void* thread_work_ts(void* arg){
  int* mutex = (int *) arg;
  while(1) {
    lock_ts(mutex);
    if (counter_ts == NB_SECTIONS_TS) {
      unlock_ts(mutex);
      break;
    }
    while(rand() > RAND_MAX/10000);
    counter_ts++;
    unlock_ts(mutex);
  }
  return NULL;
}
