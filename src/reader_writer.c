#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "../headers/reader_writer.h"
#include "../headers/test_and_test_and_set.h"
#include "../headers/test_and_set.h"
#include "../headers/tts_semaphore.h"
#include "../headers/ts_semaphore.h"

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
  return NULL;
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
  return NULL;
}

void* tts_reader(void* args){
  our_rw_args* reader_args = (our_rw_args*) args;
  for(int i = 0; i < TOREAD; i++) {
    semaphore_wait_tts(reader_args->reader_sem);
    lock_tts(reader_args->mutex_reader);
    readcount++;
    if (readcount==1) {
     semaphore_wait_tts(reader_args->db);
    }
    unlock_tts(reader_args->mutex_reader);
    semaphore_post_tts(reader_args->reader_sem);
    while(rand() > RAND_MAX/10000);
    lock_tts(reader_args->mutex_reader);
    readcount--;
    if(readcount==0){
     semaphore_post_tts(reader_args->db);
    }
    unlock_tts(reader_args->mutex_reader);
  }
  return NULL;
}

void* tts_writer(void* args){
  our_rw_args* writer_args = (our_rw_args*) args;
  for(int i = 0; i < TOWRITE; i++) {
    lock_tts(writer_args->mutex_writer);
    writer_count++;
    if (writer_count == 1){
      semaphore_wait_tts(writer_args->reader_sem);
    }
    unlock_tts(writer_args->mutex_writer);
    semaphore_wait_tts(writer_args->db);
    while(rand() > RAND_MAX/10000);
    semaphore_post_tts(writer_args->db);
    lock_tts(writer_args->mutex_writer);
    writer_count--;
    if (writer_count == 0){
      semaphore_post_tts(writer_args->reader_sem);
    }
    unlock_tts(writer_args->mutex_writer);

  }
  return NULL;
}

void* ts_reader(void* args){
  our_rw_args* reader_args = (our_rw_args*) args;
  for(int i = 0; i < TOREAD; i++) {
    semaphore_wait_ts(reader_args->reader_sem);
    lock_ts(reader_args->mutex_reader);
    readcount++;
    if (readcount==1) {
     semaphore_wait_ts(reader_args->db);
    }
    unlock_ts(reader_args->mutex_reader);
    semaphore_post_ts(reader_args->reader_sem);
    while(rand() > RAND_MAX/10000);
    lock_ts(reader_args->mutex_reader);
    readcount--;
    if(readcount==0){
     semaphore_post_ts(reader_args->db);
    }
    unlock_ts(reader_args->mutex_reader);
  }
  return NULL;
}

void* ts_writer(void* args){
  our_rw_args* writer_args = (our_rw_args*) args;
  for(int i = 0; i < TOWRITE; i++) {
    lock_ts(writer_args->mutex_writer);
    writer_count++;
    if (writer_count == 1){
      semaphore_wait_ts(writer_args->reader_sem);
    }
    unlock_ts(writer_args->mutex_writer);
    semaphore_wait_ts(writer_args->db);
    while(rand() > RAND_MAX/10000);
    semaphore_post_ts(writer_args->db);
    lock_ts(writer_args->mutex_writer);
    writer_count--;
    if (writer_count == 0){
      semaphore_post_ts(writer_args->reader_sem);
    }
    unlock_ts(writer_args->mutex_writer);

  }
  return NULL;
}
