#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "headers/test_and_test_and_set.h"


int main(int argc, char const *argv[]) {
  int *mutex;
  const int NB_THREADS = atoi(argv[1]);
  lock_init(&mutex);
  pthread_t threads[NB_THREADS];


  printf("Init \n");

  for(int i = 0; i < NB_THREADS; i++) {
    int error = pthread_create(&(threads[i]), NULL, &thread_work, (void *) mutex);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
    printf("Thread %d created\n", i);
  }

  for(int i = 0; i < NB_THREADS; i++) {
    int error = pthread_join(threads[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    printf("Thread %d joined\n", i);
  }

  lock_destroy(&mutex);

  return 0;
}
