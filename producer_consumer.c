#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>
#include <semaphore.h>

#define N 8
#define TOPRODUCE 1024

int BUFFER[N];
int to_insert = 0;
int to_remove = 0;
int NB_CONSUMERS;
int NB_PRODUCERS;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int count_prod = 0;
int count_cons = 0;

int produce_int(){
  //int nb_between_int_max_and_in_min = (rand() / RAND_MAX) * INT_MAX;
  //if (rand() > RAND_MAX / 2) nb_between_int_max_and_in_min *= -1;
  int random = 0;
  for(int i = 0; i < 32; i++) { // do it 32 times
    int randomBit = rand() > RAND_MAX / 2;  // 0 or 1
    random = random << 1; // move one bit to left
    random = random | randomBit; // set the last bit to 0 or 1
  }
  return random;
}

void insert_item(int item) {
  BUFFER[to_insert] = item;
  to_insert = (to_insert +1)%N;
}

int remove_item(){
  int item = BUFFER[to_remove];
  to_remove = (to_remove+1)%N;
  return item;
}

void* producer(void* args)
{
  int item;
  int stop = *((int *) args);
  for(int i = 0; i < stop; i++)
  {
    int item=produce_int();

    while(rand() > RAND_MAX/10000); // simulate time of producing

    sem_wait(&empty); // attente d'une place libre
    pthread_mutex_lock(&mutex);
     // section critique
     insert_item(item);
     count_prod ++;
     //while(rand() > RAND_MAX/10000);
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a une place remplie en plus
  }
}

void* consumer(void* args)
{
 int item;
 int stop = *((int *) args);
 for(int i = 0; i < stop; i++)
 {
   sem_wait(&full); // attente d'une place remplie
   pthread_mutex_lock(&mutex);
   // section critique
   item=remove_item();
   count_cons ++;
   //while(rand() > RAND_MAX/10000);
   pthread_mutex_unlock(&mutex);
   sem_post(&empty); // il y a une place libre en plus
   while(rand() > RAND_MAX/10000); // simulate time of consuming
 }
}

int main(int argc, char *argv[]) { // ./producer_consumer <consumers> <producers>
  srand(time(NULL));

  int error = pthread_mutex_init(&mutex, NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = sem_init(&empty, 0 , N);  // buffer vide
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");
  error = sem_init(&full, 0 , 0);   // buffer rempli
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  NB_CONSUMERS = atoi(argv[1]);
  NB_PRODUCERS = atoi(argv[2]);

  pthread_t consumers[NB_CONSUMERS];
  pthread_t producers[NB_PRODUCERS];

  int nb_to_produce[NB_PRODUCERS];
  for(int i = 0; i < NB_PRODUCERS; i++) {
    nb_to_produce[i] = TOPRODUCE / NB_PRODUCERS;
    if (i == NB_PRODUCERS - 1) nb_to_produce[i] += (TOPRODUCE % NB_PRODUCERS);
    error = pthread_create(&(producers[i]), NULL, &producer, (void *)&nb_to_produce[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  int nb_to_consume[NB_CONSUMERS];
  for(int i = 0; i < NB_CONSUMERS; i++) {
    nb_to_consume[i] = TOPRODUCE / NB_CONSUMERS;
    if (i == NB_CONSUMERS - 1) nb_to_consume[i] += TOPRODUCE % NB_CONSUMERS;
    error = pthread_create(&(consumers[i]), NULL, &consumer, (void *)&nb_to_consume[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_PRODUCERS; i++) {
    error = pthread_join(producers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }
  for(int i = 0; i < NB_CONSUMERS; i++) {
    error = pthread_join(consumers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }

  error = pthread_mutex_destroy(&mutex);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = sem_destroy(&empty);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  error = sem_destroy(&full);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");


  return 0;
}
