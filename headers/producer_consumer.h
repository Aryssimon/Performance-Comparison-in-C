#ifndef PRODUCER_CONSUMER
#define PRODUCER_CONSUMER
#include "our_semaphore.h"

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
  int* count;
} our_pc_args;


int produce_int();

void insert_item(int item);

int remove_item();

void* producer(void* args);

void* consumer(void* args);

void* our_producer(void* args);

void* our_consumer(void* args);


#endif //PRODUCER_CONSUMER
