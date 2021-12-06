#ifndef PRODUCER_CONSUMER
#define PRODUCER_CONSUMER

#include "tts_semaphore.h"

typedef struct {
  pthread_mutex_t* mutex;
  sem_t* empty;
  sem_t* full;
  int stop;
} pc_args;

typedef struct {
  int* mutex;
  semaphore* empty;
  semaphore* full;
  int stop;
} our_pc_args;


int produce_int();

void insert_item(int item);

int remove_item();

void* producer(void* args);

void* consumer(void* args);

void* tts_producer(void* args);

void* tts_consumer(void* args);

void* ts_producer(void* args);

void* ts_consumer(void* args);


#endif //PRODUCER_CONSUMER
