#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int CYCLES = 100000;

typedef struct {
  int index;
  int N_BAGUETTES;
  pthread_mutex_t *baguette;
} arguments;

void* philosophe(void* arg) {
  arguments *args = (arguments *) arg;
  pthread_mutex_t *baguette = args->baguette;
  int left = args->index;
  int right = (left + 1) % args->N_BAGUETTES;
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

  arguments *all_args[N];
  for(int i = 0; i < N; i++) {
    arguments *args = (arguments *) malloc(sizeof(arguments));
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

  for(int i=0; i < N; i++) {
    int error = pthread_mutex_destroy(&(baguette[i]));
    if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");
  }

  return 0;
}