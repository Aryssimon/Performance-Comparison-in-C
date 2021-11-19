#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int CYCLES = 10000;

typedef struct {
  int index;
  int N;
  pthread_mutex_t *baguette;
} arguments;

void* philosophe(void* arg) {
  arguments *args = (arguments *) arg;
  pthread_mutex_t *baguette = args->baguette;
  int left = args->index;
  int right = (left + 1) % args->N;
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
  pthread_mutex_t baguette[N];

  for(int i = 0; i < N; i++) {
    int error = pthread_mutex_init(&(baguette[i]), NULL);
    if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");
  }

  arguments *all_args[N];
  for(int i = 0; i < N; i++) {
    arguments *args = (arguments *) malloc(sizeof(arguments));
    args->index = i;
    args->N = N;
    args->baguette = baguette;
    all_args[i] = args;
    int error = pthread_create(&(phil[i]), NULL, &philosophe, (void *) args);
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
