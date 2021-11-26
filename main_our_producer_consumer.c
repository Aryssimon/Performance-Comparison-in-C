#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "headers/producer_consumer.h"
#include "headers/test_and_test_and_set.h"
#include "headers/our_semaphore.h"

int main(int argc, char *argv[]) { // ./producer_consumer <consumers> <producers>
  const int NB_CONSUMERS = atoi(argv[1]);
  const int NB_PRODUCERS = atoi(argv[2]);
  const int TOPRODUCE = 1024;

  int* mutex;
  semaphore* empty;
  semaphore* full;

  srand(time(NULL));

  int error = lock_init(&mutex);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = our_sem_init(&empty,8);  // buffer vide
  if (error != 0) fprintf(stderr, "sem_init failed\n");
  error = our_sem_init(&full,0);   // buffer rempli
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  pthread_t consumers[NB_CONSUMERS];
  pthread_t producers[NB_PRODUCERS];

  int *count_prod;
  *count_prod = 0;
  int nb_to_produce[NB_PRODUCERS];
  our_pc_args *all_prod_args[NB_PRODUCERS];
  for(int i = 0; i < NB_PRODUCERS; i++) {
    nb_to_produce[i] = TOPRODUCE / NB_PRODUCERS;
    if (i == NB_PRODUCERS - 1) nb_to_produce[i] += (TOPRODUCE % NB_PRODUCERS);
    our_pc_args *prod_args = (our_pc_args *) malloc(sizeof(our_pc_args));
    prod_args->mutex = mutex;
    prod_args->empty = empty;
    prod_args->full = full;
    prod_args->stop = nb_to_produce[i];
    prod_args->count = count_prod;
    all_prod_args[i] = prod_args;
    error = pthread_create(&(producers[i]), NULL, &our_producer, (void *)all_prod_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  int *count_cons;
  *count_cons = 0;
  int nb_to_consume[NB_CONSUMERS];
  our_pc_args *all_cons_args[NB_CONSUMERS];
  for(int i = 0; i < NB_CONSUMERS; i++) {
    nb_to_consume[i] = TOPRODUCE / NB_CONSUMERS;
    if (i == NB_CONSUMERS - 1) nb_to_consume[i] += TOPRODUCE % NB_CONSUMERS;
    our_pc_args *cons_args = (our_pc_args *) malloc(sizeof(our_pc_args));
    cons_args->mutex = mutex;
    cons_args->empty = empty;
    cons_args->full = full;
    cons_args->stop = nb_to_consume[i];
    cons_args->count = count_cons;
    all_cons_args[i] = cons_args;
    error = pthread_create(&(consumers[i]), NULL, &our_consumer, (void *)all_cons_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_PRODUCERS; i++) {
    error = pthread_join(producers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_prod_args[i]);
  }
  for(int i = 0; i < NB_CONSUMERS; i++) {
    error = pthread_join(consumers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_cons_args[i]);
  }
  printf("items produced : %d\n",*count_prod);
  printf("items consumed : %d\n",*count_cons);

  error = lock_destroy(&mutex);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = our_sem_destroy(&empty);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  error = our_sem_destroy(&full);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");


  return 0;
}