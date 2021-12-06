#ifndef PHILOSOPHE
#define PHILOSOPHE


typedef struct {
  int index;
  int N_BAGUETTES;
  pthread_mutex_t *baguette;
} phil_args;

typedef struct {
  int index;
  int N_BAGUETTES;
  int **baguette;
} our_phil_args;

void* philosophe(void* args);

void* tts_philosophe(void* args);

void* ts_philosophe(void* args);


#endif //PHILOSOPHE
