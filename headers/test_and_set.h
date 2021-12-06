#ifndef TEST_AND_SET
#define TEST_AND_SET


int test_and_set_ts(int *locker);

int lock_init_ts(int **locker);

int lock_destroy_ts(int **locker);

int lock_ts(int *locker);

int unlock_ts(int *locker);

void* thread_work_ts(void* arg);


#endif //TEST_AND_SET
