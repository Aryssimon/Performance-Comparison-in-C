#ifndef PHILOSOPHE
#define PHILOSOPHE


typedef struct {
  int index;
  int N_BAGUETTES;
  pthread_mutex_t *baguette;
} phil_args;

void* philosophe(void* args);


#endif //PHILOSOPHE
