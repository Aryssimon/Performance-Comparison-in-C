#include <stdlib.h>
#include <pthread.h>

const int CYCLES = 10000;

pthread_t *phil;
pthread_mutex_t *baguette;

int main(int argc, char *argv[]) {
  int N = atoi(argv[1]);
  pthread_t init_phil[N];
  pthread_mutex_t init_baguette[N];
  phil = init_phil;
  baguette = init_baguette;
}
