pthread_mutex_t mutex;
pthread_mutex_t mutex_writer_count;
pthread_mutex_t z;
sem_t db;  // accès à la db
sem_t reader_sem;
sem_init(&reader_sem,0,1);
int readcount=0; // nombre de readers
int writer_count = 0
sem_init(&db, NULL, 1);
pthread_mutex_init(&mutex,NULL);
pthread_mutex_init(&mutex_writer_count,NULL);
pthread_mutex_init(&z,NULL)

void reader(void)
{
  while(true){
   pthread_mutex_lock(&z);
   sem_wait(&reader_sem)
   pthread_mutex_lock(&mutex);
     readcount++;
     if (readcount==1)
     { // arrivée du premier reader
       sem_wait(&db);
     }
   pthread_mutex_unlock(&mutex);
   sem_post(&reader_sem);
   pthread_mutex_unlock(&z);
   read_database();
   pthread_mutex_lock(&mutex);
     // section critique
     readcount--;
     if(readcount==0)
     { // départ du dernier reader
       sem_post(&db);
     }
   pthread_mutex_unlock(&mutex);
   process_data();
 }
}

void writer(void)
{
 while(true)
 {
   prepare_data();
   pthread_mutex_lock(&mutex_writer_count);
   writer_count++;
   if (writer_count == 1) {
       sem_wait(&reader_sem);
   }
   pthread_mutex_unlock(&mutex_writer);
   sem_wait(&db);
     // section critique, un seul writer à la fois
     write_database();
     pthread_mutex_lock(&mutex_writer);
     writer_count --;
     if (writer_count == 0){
         sem_post(&reader_sem);
     }
     pthread_mutex_unlock(&mutex_writer);
   sem_post(&db);
   }
 }
}

int main(int argc, char *argv[]) {
  return 0;
}
