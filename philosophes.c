#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int CYCLES = 10000;
const int N = 5;

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
//  N = atoi(argv[1]);
  pthread_t init_phil[5] = {0};
  pthread_mutex_t init_baguette[N];
  phil = init_phil;
  baguette = init_baguette;

  for(int i = 0; i < N; i++) {
    int error = pthread_mutex_init(&(baguette[i]), NULL);
    if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");
  }

  for(int i = 0; i < N; i++) {
    int error = pthread_create(&(phil[i]), NULL, &philosophe, (void *) &i);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < N; i++) {
    fprintf(stderr, "pthread_join %p\n", phil[i]);
    int error = pthread_join(phil[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }
  /*
  for(int i; i < N; i++) {
    int error = pthread_mutex_destroy(&(baguette[i]));
    if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");
  }
  */
  return 0;
}
