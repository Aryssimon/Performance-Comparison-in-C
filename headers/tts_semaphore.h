#ifndef TTS_SEMAPHORE
#define TTS_SEMAPHORE


typedef struct {
  int val;
  int* mutex;
  int* wait_mutex;
} semaphore;

int tts_sem_init(semaphore **sem, unsigned int value);

int tts_sem_destroy(semaphore **sem);

int semaphore_post_tts(semaphore* sem);

int semaphore_wait_tts(semaphore* sem);


#endif //TTS_SEMAPHORE
