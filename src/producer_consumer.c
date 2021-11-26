#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../headers/producer_consumer.h"
#include "../headers/test_and_test_and_set.h"
#include "../headers/our_semaphore.h"

#define N 8

int BUFFER[N];
int to_insert = 0;
int to_remove = 0;


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
  pc_args* prod_args = (pc_args *) args;
  for(int i = 0; i < prod_args->stop; i++)
  {
    int item=produce_int();

    while(rand() > RAND_MAX/10000); // simulate time of producing

    sem_wait(prod_args->empty); // attente d'une place libre
    pthread_mutex_lock(prod_args->mutex);
    // section critique
    insert_item(item);
    //while(rand() > RAND_MAX/10000);
    pthread_mutex_unlock(prod_args->mutex);
    sem_post(prod_args->full); // il y a une place remplie en plus
  }
  pthread_exit (NULL);
}

void* consumer(void* args)
{
  int item;
  pc_args* cons_args = (pc_args *) args;
  for(int i = 0; i < cons_args->stop; i++)
  {
    sem_wait(cons_args->full); // attente d'une place remplie
    pthread_mutex_lock(cons_args->mutex);
    // section critique
    item=remove_item();
    //while(rand() > RAND_MAX/10000);
    pthread_mutex_unlock(cons_args->mutex);
    sem_post(cons_args->empty); // il y a une place libre en plus
    while(rand() > RAND_MAX/10000); // simulate time of consuming
  }
  pthread_exit (NULL);
}

void* our_producer(void* args){
  int item;
  our_pc_args* prod_args = (our_pc_args *) args;
  for(int i = 0; i < prod_args->stop; i++)
  {
    int item=produce_int();

    while(rand() > RAND_MAX/10000); // simulate time of producing

    semaphore_wait(prod_args->empty); // attente d'une place libre
    lock_tts(prod_args->mutex);
    printf("item produced\n");
    // section critique
    insert_item(item);
    *(prod_args->count) = *(prod_args->count)+1;
    unlock_tts(prod_args->mutex);
    semaphore_post(prod_args->full); // il y a une place remplie en plus
  }
  pthread_exit (NULL);
}

void* our_consumer(void* args){
  int item;
  our_pc_args* cons_args = (our_pc_args *) args;
  for(int i = 0; i < cons_args->stop; i++)
  {
    semaphore_wait(cons_args->full); // attente d'une place remplie
    lock_tts(cons_args->mutex);
    // section critique
    item=remove_item();
    printf("item consumed\n");
    *(cons_args->count) = *(cons_args->count)+1;
    unlock_tts(cons_args->mutex);
    semaphore_post(cons_args->empty); // il y a une place libre en plus
    while(rand() > RAND_MAX/10000); // simulate time of consuming
  }
  pthread_exit (NULL);
}
