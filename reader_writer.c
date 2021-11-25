#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TOWRITE 640
#define TOREAD 2560

int NB_WRITERS;
int NB_READERS;

pthread_mutex_t mutex;
pthread_mutex_t mutex_writer_count;
pthread_mutex_t z;
sem_t db;
sem_t reader_sem;
int readcount = 0;
int writer_count = 0;

void* reader(void* args) {
  for(int i = 0; i < TOREAD; i++) {
    sem_wait(&reader_sem);
    pthread_mutex_lock(&mutex);
    readcount++;
    if (readcount==1) {
     sem_wait(&db);
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&reader_sem);
    while(rand() > RAND_MAX/10000);
    pthread_mutex_lock(&mutex);
    readcount--;
    if(readcount==0){
     sem_post(&db);
    }
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit (NULL);
}

void* writer(void* args) {
  for(int i = 0; i < TOWRITE; i++) {
    pthread_mutex_lock(&mutex_writer_count);
    writer_count++;
    if (writer_count == 1){
      sem_wait(&reader_sem);
    }
    pthread_mutex_unlock(&mutex_writer_count);
    sem_wait(&db);
    while(rand() > RAND_MAX/10000);
    sem_post(&db);
    pthread_mutex_lock(&mutex_writer_count);
    writer_count--;
    if (writer_count == 0){
      sem_post(&reader_sem);
    }
    pthread_mutex_unlock(&mutex_writer_count);

  }
  pthread_exit (NULL);
}

int main(int argc, char *argv[]) {
  NB_WRITERS = atoi(argv[1]);
  NB_READERS = atoi(argv[2]);

  int error = sem_init(&reader_sem, 0, 1);
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  error = sem_init(&db, 0, 1);
  if (error != 0) fprintf(stderr, "sem_init failed\n");

  error = pthread_mutex_init(&mutex,NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = pthread_mutex_init(&mutex_writer_count,NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  error = pthread_mutex_init(&z,NULL);
  if (error != 0) fprintf(stderr, "pthread_mutex_init failed\n");

  pthread_t writers[NB_WRITERS];
  pthread_t readers[NB_READERS];

  for(int i = 0; i < NB_WRITERS; i++) {
    error = pthread_create(&(writers[i]), NULL, &writer, (void *) NULL);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_READERS; i++) {
    error = pthread_create(&(readers[i]), NULL, &reader, (void *) NULL);
    if (error != 0) fprintf(stderr, "pthread_create failed\n");
  }

  for(int i = 0; i < NB_WRITERS; i++) {
    error = pthread_join(writers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }
  for(int i = 0; i < NB_READERS; i++) {
    error = pthread_join(readers[i], NULL);
    if (error != 0) fprintf(stderr, "pthread_join failed\n");
  }

  error = pthread_mutex_destroy(&mutex);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = pthread_mutex_destroy(&mutex_writer_count);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = pthread_mutex_destroy(&z);
  if (error != 0) fprintf(stderr, "pthread_mutex_destroy failed\n");

  error = sem_destroy(&reader_sem);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  error = sem_destroy(&db);
  if (error != 0) fprintf(stderr, "sem_destroy failed\n");

  return 0;
}
