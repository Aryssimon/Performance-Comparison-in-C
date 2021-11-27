#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "headers/producer_consumer.h"


int main(int argc, char *argv[]) {
  const int NB_CONSUMERS = atoi(argv[1]);
  const int NB_PRODUCERS = atoi(argv[2]);
  const int TOPRODUCE = 1024;

  pthread_mutex_t mutex;
  sem_t empty;
  sem_t full;

  srand(time(NULL));

  int error = pthread_mutex_init(&mutex, NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = sem_init(&empty, 0 , 8);
  if (error != 0) fprintf(stderr, "sem_init failed\n");
  error = sem_init(&full, 0 , 0);
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  pthread_t consumers[NB_CONSUMERS];
  pthread_t producers[NB_PRODUCERS];

  int nb_to_produce[NB_PRODUCERS];
  pc_args *all_prod_args[NB_PRODUCERS];
  for(int i = 0; i < NB_PRODUCERS; i++) {
    nb_to_produce[i] = TOPRODUCE / NB_PRODUCERS;
    if (i == NB_PRODUCERS - 1) nb_to_produce[i] += (TOPRODUCE % NB_PRODUCERS);
    pc_args *prod_args = (pc_args *) malloc(sizeof(pc_args));
    if (prod_args == NULL) fprintf(stderr, "malloc failed\n");
    prod_args->mutex = &mutex;
    prod_args->empty = &empty;
    prod_args->full = &full;
    prod_args->stop = nb_to_produce[i];
    all_prod_args[i] = prod_args;
    error = pthread_create(&(producers[i]), NULL, &producer, (void *)all_prod_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  int nb_to_consume[NB_CONSUMERS];
  pc_args *all_cons_args[NB_CONSUMERS];
  for(int i = 0; i < NB_CONSUMERS; i++) {
    nb_to_consume[i] = TOPRODUCE / NB_CONSUMERS;
    if (i == NB_CONSUMERS - 1) nb_to_consume[i] += TOPRODUCE % NB_CONSUMERS;
    pc_args *cons_args = (pc_args *) malloc(sizeof(pc_args));
    if (cons_args == NULL) fprintf(stderr, "malloc failed\n");
    cons_args->mutex = &mutex;
    cons_args->empty = &empty;
    cons_args->full = &full;
    cons_args->stop = nb_to_consume[i];
    all_cons_args[i] = cons_args;
    error = pthread_create(&(consumers[i]), NULL, &consumer, (void *)all_cons_args[i]);
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

  error = pthread_mutex_destroy(&mutex);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = sem_destroy(&empty);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  error = sem_destroy(&full);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");


  return 0;
}
