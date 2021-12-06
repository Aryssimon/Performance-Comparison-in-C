#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "headers/test_and_set.h"


int main(int argc, char const *argv[]) {
  const int NB_THREADS = atoi(argv[1]);

  int *mutex;
  lock_init_ts(&mutex);

  pthread_t threads[NB_THREADS];
  for(int i = 0; i < NB_THREADS; i++) {
    int error = pthread_create(&(threads[i]), NULL, &thread_work_ts, (void *) mutex);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_THREADS; i++) {
    int error = pthread_join(threads[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }

  lock_destroy_ts(&mutex);

  return 0;
}
