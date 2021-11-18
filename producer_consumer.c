#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>

#define N 8
#define TOPRODUCE 1024

const int BUFFER[N];
int to_insert = 0;
int to_remove = 0;
const NB_CONSUMERS;
const NB_PRODUCERS;

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

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

pthread_mutex_init(&mutex, NULL);
sem_init(&empty, 0 , N);  // buffer vide
sem_init(&full, 0 , 0);   // buffer vide

void producer(void)
{
  int item;
  for(int i = 0; i < TOPRODUCE; i++)
  {
    int item=produce_int();

    while(rand() > RAND_MAX/10000); // simulate time of producing

    sem_wait(&empty); // attente d'une place libre
    pthread_mutex_lock(&mutex);
     // section critique
     insert_item(item);
     while(rand() > RAND_MAX/10000);
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a une place remplie en plus
  }
}

void consumer(void)
{
 int item;
 for(int i = 0; i < TOPRODUCE; i++)
 {
   sem_wait(&full); // attente d'une place remplie
   pthread_mutex_lock(&mutex);
   // section critique
   item=remove_item();
   while(rand() > RAND_MAX/10000);
   pthread_mutex_unlock(&mutex);
   sem_post(&empty); // il y a une place libre en plus
   while(rand() > RAND_MAX/10000); // simulate time of consuming
 }
}

int main(int argc, char *argv[]) { // ./producer_consumer <consumers> <producers>
  NB_CONSUMERS = atoi(argv[1]);
  NB_PRODUCERS = atoi(argv[2]);

  pthread_t consumers[NB_CONSUMERS];
  pthread_t producers[NB_PRODUCERS];

  for(int i = 0; i < NB_CONSUMERS; i++) {
    int error = pthread_create(&(phil[i]), NULL, &consumer, NULL);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
    printf("Create thread %d\n", i);
  }
  for(int i = 0; i < NB_PRODUCERS; i++) {
    int error = pthread_create(&(phil[i]), NULL, &producer, NULL);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
    printf("Create thread %d\n", i);
  }

  return 0;
}
