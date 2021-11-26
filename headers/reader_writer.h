#ifndef READER_WRITER
#define READER_WRITER


typedef struct {
  pthread_mutex_t* mutex_reader;
  pthread_mutex_t* mutex_writer;
  sem_t* db;
  sem_t* reader_sem;
} rw_args;

void* reader(void* args);

void* writer(void* args);


#endif //READER_WRITER
