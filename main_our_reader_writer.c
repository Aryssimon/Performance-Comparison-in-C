#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "headers/reader_writer.h"
#include "headers/test_and_test_and_set.h"
#include "headers/our_semaphore.h"


int main(int argc, char *argv[]) {
  const int NB_WRITERS = atoi(argv[1]);
  const int NB_READERS = atoi(argv[2]);

  int* mutex_reader;
  int* mutex_writer;
  semaphore* db;
  semaphore* reader_sem;

  our_sem_init(&reader_sem, 1);
  our_sem_init(&db, 1);

  lock_init(&mutex_reader);
  lock_init(&mutex_writer);

  pthread_t writers[NB_WRITERS];
  pthread_t readers[NB_READERS];

  our_rw_args *all_writers_args[NB_WRITERS];
  for(int i = 0; i < NB_WRITERS; i++) {
    our_rw_args *writer_args = (our_rw_args *) malloc(sizeof(our_rw_args));
    if (writer_args == NULL) fprintf(stderr, "malloc failed\n");
    writer_args->mutex_reader = mutex_reader;
    writer_args->mutex_writer = mutex_writer;
    writer_args->db = db;
    writer_args->reader_sem = reader_sem;
    all_writers_args[i] = writer_args;
    int error = pthread_create(&(writers[i]), NULL, &our_writer, (void *) all_writers_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  our_rw_args *all_readers_args[NB_READERS];
  for(int i = 0; i < NB_READERS; i++) {
    our_rw_args *reader_args = (our_rw_args *) malloc(sizeof(our_rw_args));
    if (reader_args == NULL) fprintf(stderr, "malloc failed\n");
    reader_args->mutex_reader = mutex_reader;
    reader_args->mutex_writer = mutex_writer;
    reader_args->db = db;
    reader_args->reader_sem = reader_sem;
    all_readers_args[i] = reader_args;
    int error = pthread_create(&(readers[i]), NULL, &our_reader, (void *) all_readers_args[i]);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_WRITERS; i++) {
    int error = pthread_join(writers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_writers_args[i]);
  }
  for(int i = 0; i < NB_READERS; i++) {
    int error = pthread_join(readers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
    free(all_readers_args[i]);
  }

  lock_destroy(&mutex_reader);
  lock_destroy(&mutex_writer);

  our_sem_destroy(&reader_sem);
  our_sem_destroy(&db);

  return 0;
}
