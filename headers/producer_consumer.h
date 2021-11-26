#ifndef PRODUCER_CONSUMER
#define PRODUCER_CONSUMER


typedef struct {
  pthread_mutex_t* mutex;
  sem_t* empty;
  sem_t* full;
  int stop;
} pc_args;


int produce_int();

void insert_item(int item);

int remove_item();

void* producer(void* args);

void* consumer(void* args);


#endif //PRODUCER_CONSUMER
