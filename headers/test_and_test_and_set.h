#ifndef TEST_AND_TEST_AND_SET
#define TEST_AND_TEST_AND_SET


int test_and_set_tts(int *locker);

int lock_init_tts(int **locker);

int lock_destroy_tts(int **locker);

int lock_tts(int *locker);

int unlock_tts(int *locker);

void* thread_work_tts(void* arg);


#endif //TEST_AND_TEST_AND_SET
