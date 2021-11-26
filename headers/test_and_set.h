#ifndef TEST_AND_SET
#define TEST_AND_SET


int test_and_set(int *locker);

int lock_init(int **locker);

int lock_destroy(int **locker);

int lock_ts(int *locker);

int unlock(int *locker);

void* thread_work(void* arg);






#endif //TEST_AND_SET
