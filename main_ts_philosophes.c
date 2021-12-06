#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "headers/philosophes.h"
#include "headers/test_and_set.h"

int main(int argc, char *argv[]) {
  const int N = atoi(argv[1]);
  pthread_t phil[N];
  int N_BAGUETTES = N;
  if (N == 1) N_BAGUETTES++;
  int* baguette[N_BAGUETTES];

  for(int i = 0; i < N_BAGUETTES; i++) {
    lock_init_ts(&(baguette[i]));
  }

  our_phil_args *all_args[N];
  for(int i = 0; i < N; i++) {
    our_phil_args *args = (our_phil_args *) malloc(sizeof(our_phil_args));
    if (args == NULL) fprintf(stderr, "malloc failed\n");
    args->index = i;
    args->N_BAGUETTES = N_BAGUETTES;
    args->baguette = baguette;
    all_args[i] = args;
    int error = pthread_create(&(phil[i]), NULL, &ts_philosophe, (void *) all_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_join(phil[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_args[i]);
  }

  for(int i = 0; i < N; i++) {
    lock_destroy_ts(&(baguette[i]));
  }

  return 0;
}
