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
  int random = 0;
  for(int i = 0; i < 32; i++) { // do it 32 times
    int randomBit = rand() > RAND_MAX / 2;  // 0 or 1
    random = random << 1; // move one bit to left
    random = random | randomBit; // set the first bit to 0 or 1
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
    item=produce_int();
    while(rand() > RAND_MAX/10000);

    sem_wait(prod_args->empty);

    pthread_mutex_lock(prod_args->mutex);
    insert_item(item);
    pthread_mutex_unlock(prod_args->mutex);

    sem_post(prod_args->full);
  }
  pthread_exit (NULL);
}

void* consumer(void* args)
{
  pc_args* cons_args = (pc_args *) args;
  for(int i = 0; i < cons_args->stop; i++)
  {
    sem_wait(cons_args->full);

    pthread_mutex_lock(cons_args->mutex);
    remove_item();
    pthread_mutex_unlock(cons_args->mutex);

    sem_post(cons_args->empty);
    while(rand() > RAND_MAX/10000);
  }
  pthread_exit (NULL);
}

void* our_producer(void* args){
  int item;
  our_pc_args* prod_args = (our_pc_args *) args;
  for(int i = 0; i < prod_args->stop; i++)
  {
    item=produce_int();
    while(rand() > RAND_MAX/10000);

    semaphore_wait(prod_args->empty);

    lock_tts(prod_args->mutex);
    insert_item(item);
    unlock_tts(prod_args->mutex);

    semaphore_post(prod_args->full);
  }
  pthread_exit (NULL);
}

void* our_consumer(void* args){
  our_pc_args* cons_args = (our_pc_args *) args;
  for(int i = 0; i < cons_args->stop; i++)
  {
    semaphore_wait(cons_args->full);

    lock_tts(cons_args->mutex);
    remove_item();
    unlock_tts(cons_args->mutex);

    semaphore_post(cons_args->empty);
    while(rand() > RAND_MAX/10000);
  }
  pthread_exit (NULL);
}
