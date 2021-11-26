#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "headers/philosophes.h"


int main(int argc, char *argv[]) {
  const int N = atoi(argv[1]);
  pthread_t phil[N];
  int N_BAGUETTES = N;
  if (N == 1) N_BAGUETTES++;
  pthread_mutex_t baguette[N_BAGUETTES];

  for(int i = 0; i < N_BAGUETTES; i++) {
    int error = pthread_mutex_init(&(baguette[i]), NULL);
    if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");
  }

  phil_args *all_args[N];
  for(int i = 0; i < N; i++) {
    phil_args *args = (phil_args *) malloc(sizeof(phil_args));
    args->index = i;
    args->N_BAGUETTES = N_BAGUETTES;
    args->baguette = baguette;
    all_args[i] = args;
    int error = pthread_create(&(phil[i]), NULL, &philosophe, (void *) all_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_join(phil[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_args[i]);
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_mutex_destroy(&(baguette[i]));
    if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");
  }

  return 0;
}
