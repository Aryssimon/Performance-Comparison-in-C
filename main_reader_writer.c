#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "headers/reader_writer.h"


int main(int argc, char *argv[]) {
  const int NB_WRITERS = atoi(argv[1]);
  const int NB_READERS = atoi(argv[2]);

  pthread_mutex_t mutex_reader;
  pthread_mutex_t mutex_writer;
  sem_t db;
  sem_t reader_sem;

  int error = sem_init(&reader_sem, 0, 1);
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  error = sem_init(&db, 0, 1);
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  error = pthread_mutex_init(&mutex_reader,NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = pthread_mutex_init(&mutex_writer,NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  pthread_t writers[NB_WRITERS];
  pthread_t readers[NB_READERS];


  rw_args *all_writers_args[NB_WRITERS];
  for(int i = 0; i < NB_WRITERS; i++) {
    rw_args *writer_args = (rw_args *) malloc(sizeof(rw_args));
    writer_args->mutex_reader = &mutex_reader;
    writer_args->mutex_writer = &mutex_writer;
    writer_args->db = &db;
    writer_args->reader_sem = &reader_sem;
    all_writers_args[i] = writer_args;
    error = pthread_create(&(writers[i]), NULL, &writer, (void *) all_writers_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }


  rw_args *all_readers_args[NB_READERS];
  for(int i = 0; i < NB_READERS; i++) {
    rw_args *reader_args = (rw_args *) malloc(sizeof(rw_args));
    reader_args->mutex_reader = &mutex_reader;
    reader_args->mutex_writer = &mutex_writer;
    reader_args->db = &db;
    reader_args->reader_sem = &reader_sem;
    all_readers_args[i] = reader_args;
    error = pthread_create(&(readers[i]), NULL, &reader, (void *) all_readers_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_WRITERS; i++) {
    error = pthread_join(writers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_writers_args[i]);
  }
  for(int i = 0; i < NB_READERS; i++) {
    error = pthread_join(readers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_readers_args[i]);
  }

  error = pthread_mutex_destroy(&mutex_reader);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = pthread_mutex_destroy(&mutex_writer);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = sem_destroy(&reader_sem);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  error = sem_destroy(&db);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  return 0;
}
