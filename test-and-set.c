#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>

int mutexValue = 0;
int *mutex = &mutexValue;
int counter = 0;
const int NB_SECTIONS = 6400;

int test_and_set(int *locker){
  int value = 1;
  asm (
       "xchg %2, %3"
  : "+q" (value), "+m"(*locker));
  return value;
}

int lock(int *locker){
  while(test_and_set(locker) == 1);
  return 0;
}

int unlock(int *locker){
  asm ("mov $0, %%eax\n"
       "xchg %%eax, %0"
  : "+m"(*locker)
  :
  : "eax");

  return 0;
}

void* thread_work(void* arg){
  int stop = *((int *) arg);
  printf("Thread start\n");
  int personal_counter = 0;
  while(1) {
    printf("Before lock \n");
    lock(mutex);
    printf("Inside lock \n");
    if (counter == NB_SECTIONS) {
      unlock(mutex);
      break;
    }
    printf("Section critique start\n");
    while(rand() > RAND_MAX/10000);
    counter++;
    stop--;
    personal_counter++;
    printf("Section critique end\n");
    unlock(mutex);
    printf("Unlock \n");
  }
  printf("personal counter = %d\n",personal_counter);
  pthread_exit (NULL);
}

int main(int argc, char const *argv[]) {
  const int NB_THREADS = atoi(argv[1]);
  const int NB_PER_THREAD = NB_SECTIONS / NB_THREADS;
  const int ADD_TO_LAST = NB_SECTIONS % NB_THREADS;
  pthread_t threads[NB_THREADS];

  int executions[NB_THREADS];

  printf("Init \n");

  for(int i = 0; i < NB_THREADS; i++) {
    executions[i] = NB_PER_THREAD;
    if (i == NB_THREADS - 1) executions[i] += ADD_TO_LAST;
    int error = pthread_create(&(threads[i]), NULL, &thread_work, (void *) &executions[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
    printf("Thread %d created\n", i);
  }

  for(int i = 0; i < NB_THREADS; i++) {
    int error = pthread_join(threads[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    printf("Thread %d joined\n", i);
  }

  printf("Counter: %d\n", counter);

  return 0;
}
