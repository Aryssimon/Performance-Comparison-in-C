#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../headers/reader_writer.h"

#define TOWRITE 640
#define TOREAD 2560

int readcount = 0;
int writer_count = 0;

void* reader(void* args) {
  rw_args* reader_args = (rw_args*) args;
  for(int i = 0; i < TOREAD; i++) {
    sem_wait(reader_args->reader_sem);
    pthread_mutex_lock(reader_args->mutex_reader);
    readcount++;
    if (readcount==1) {
     sem_wait(reader_args->db);
    }
    pthread_mutex_unlock(reader_args->mutex_reader);
    sem_post(reader_args->reader_sem);
    while(rand() > RAND_MAX/10000);
    pthread_mutex_lock(reader_args->mutex_reader);
    readcount--;
    if(readcount==0){
     sem_post(reader_args->db);
    }
    pthread_mutex_unlock(reader_args->mutex_reader);
  }
  pthread_exit (NULL);
}

void* writer(void* args) {
  rw_args* writer_args = (rw_args*) args;
  for(int i = 0; i < TOWRITE; i++) {
    pthread_mutex_lock(writer_args->mutex_writer);
    writer_count++;
    if (writer_count == 1){
      sem_wait(writer_args->reader_sem);
    }
    pthread_mutex_unlock(writer_args->mutex_writer);
    sem_wait(writer_args->db);
    while(rand() > RAND_MAX/10000);
    sem_post(writer_args->db);
    pthread_mutex_lock(writer_args->mutex_writer);
    writer_count--;
    if (writer_count == 0){
      sem_post(writer_args->reader_sem);
    }
    pthread_mutex_unlock(writer_args->mutex_writer);

  }
  pthread_exit (NULL);
}
