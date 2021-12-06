#ifndef TS_SEMAPHORE
#define TS_SEMAPHORE

#include "tts_semaphore.h"

int ts_sem_init(semaphore **sem, unsigned int value);

int ts_sem_destroy(semaphore **sem);

int semaphore_post_ts(semaphore* sem);

int semaphore_wait_ts(semaphore* sem);

#endif //TS_SEMAPHORE
