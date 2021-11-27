#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../headers/our_semaphore.h"
#include "../headers/test_and_test_and_set.h"


semaphore* our_sem;
/*int counter = 0;
const int NB_SECTIONS = 1024;*/


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

/*void* semaphore_work(void* arg){
  int threadNo = *((int *)arg);
  printf("Thread start %d\n", threadNo);
  int personal_counter = 0;
  while(1) {
    semaphore_wait(our_sem);
    printf("Inside lock %d\n", threadNo);
    if (counter == NB_SECTIONS) {
      printf("Exit thread: %d\n", threadNo);
      semaphore_post(our_sem);
      break;
    }
    printf("Section critique start %d\n", threadNo);
    while(rand() > RAND_MAX/10000);
    counter++;
    personal_counter++;
    printf("Section critique end %d\n", threadNo);
    semaphore_post(our_sem);
  }
  printf("personal counter = %d\n",personal_counter);
  pthread_exit (NULL);
}*/

/*int main(int argc, char const *argv[]) {
  const int NB_THREADS = atoi(argv[1]);
  const int NB_PER_THREAD = NB_SECTIONS / NB_THREADS;
  const int ADD_TO_LAST = NB_SECTIONS % NB_THREADS;
  our_sem_init(&our_sem, 1);
  pthread_t threads[NB_THREADS];

  int executions[NB_THREADS];

  printf("Init \n");

  for(int i = 0; i < NB_THREADS; i++) {
    executions[i] = i;
    //if (i == NB_THREADS - 1) executions[i] += ADD_TO_LAST;
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

  our_sem_destroy(&our_sem);

  return 0;
}*/
