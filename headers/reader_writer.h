#ifndef READER_WRITER
#define READER_WRITER
#include "tts_semaphore.h"

typedef struct {
  pthread_mutex_t* mutex_reader;
  pthread_mutex_t* mutex_writer;
  sem_t* db;
  sem_t* reader_sem;
} rw_args;

typedef struct {
  int* mutex_reader;
  int* mutex_writer;
  semaphore* db;
  semaphore* reader_sem;
} our_rw_args;

void* reader(void* args);

void* writer(void* args);

void* tts_reader(void* args);

void* tts_writer(void* args);

void* ts_reader(void* args);

void* ts_writer(void* args);


#endif //READER_WRITER
