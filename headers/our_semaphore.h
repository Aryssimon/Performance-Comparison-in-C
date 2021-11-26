#ifndef OUR_SEMAPHORE
#define OUR_SEMAPHORE


typedef struct {
  int val;
  int* mutex;
  int* wait_mutex;
} semaphore;

int our_sem_init(semaphore **sem, unsigned int value);

int our_sem_destroy(semaphore **sem);

int semaphore_post(semaphore* sem);

int semaphore_wait(semaphore* sem);

void* semaphore_work(void* arg);


#endif //OUR_SEMAPHORE
