#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int CYCLES = 10000;
int N;
pthread_t *phil;
pthread_mutex_t *baguette;

void* philosophe(void* arg) {
  int *id=(int *) arg;
  int left = *id;
  int right = (left + 1) % N;
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
  N = atoi(argv[1]);
  phil = (pthread_t*) malloc(sizeof(pthread_t) * (N + 1));
  baguette = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * (N + 1));

  for(int i = 0; i < N; i++) {
    int error = pthread_mutex_init(&(baguette[i]), NULL);
    if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");
    printf("Init mutex %d\n", i);
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_create(&(phil[i]), NULL, &philosophe, (void *) &i);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
    printf("Create thread %d\n", i);
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_join(phil[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    printf("Join thread %d\n", i);
  }
  printf("Before first destroy and after last join\n");
  for(int i=0; i < N; i++) {
    int error = pthread_mutex_destroy(&(baguette[i]));
    if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");
    printf("Destroy mutex %d\n", i);
  }

  printf("All joined \n");

  free(phil);
  free(baguette);

  printf("All fred \n");

  return 0;
}
