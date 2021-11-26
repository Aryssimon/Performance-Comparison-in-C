#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../headers/test_and_test_and_set.h"

int counter = 0;
const int NB_SECTIONS = 1024;

int test_and_set(int *locker){
  int value = 1;
  asm ("xchgl %0, %1"
  : "+q" (value), "+m"(*locker));
  return value;
}

int lock_init(int **locker){
  *locker = (int*) malloc(sizeof(int));
  *(*locker) = 0;
  return 0;
}

int lock_destroy(int **locker) {
  free(*locker);
  return 0;
}

int lock_tts(int *locker){
  while(test_and_set(locker)){
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


void* thread_work(void* arg){
  int *mutex = (int *) arg;
  printf("Thread start\n");
  int personal_counter = 0;
  while(1) {
    printf("Before lock \n");
    lock_tts(mutex);
    printf("Inside lock \n");
    if (counter == NB_SECTIONS) {
      unlock_tts(mutex);
      break;
    }
    printf("Section critique start\n");
    while(rand() > RAND_MAX/10000);
    counter++;
    personal_counter++;
    printf("Section critique end\n");
    unlock_tts(mutex);
    printf("Unlock \n");
  }
  printf("personal counter = %d\n",personal_counter);
  pthread_exit (NULL);
}
