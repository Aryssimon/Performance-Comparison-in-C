#ifndef TEST_AND_TEST_AND_SET
#define TEST_AND_TEST_AND_SET


int test_and_set(int *locker);

int lock_init(int **locker);

int lock_destroy(int **locker);

int lock_tts(int *locker);

int unlock_tts(int *locker);

void* thread_work(void* arg);


#endif //TEST_AND_TEST_AND_SET
