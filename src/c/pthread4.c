/*
 * mutex
 */

#include "common.h"

int glob = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
const int loop = 100000000;
time_t start, end;
pthread_t tid;

void func() {
    for (int j = 0; j < loop; j++) {
        if (pthread_mutex_lock(&mtx) != 0) err_exit(errno, 1);

        glob++;

        if (pthread_mutex_unlock(&mtx) != 0) err_exit(errno, 1);
    }
}

void func2() {
    for (int j = 0; j < loop; j++) glob++;
}

int main() {
    start = time(NULL);
    if (pthread_create(&tid, NULL, (start_rtn)func, NULL) != 0) err_exit(errno, 1);
    func();
    if (pthread_join(tid, NULL) != 0) err_exit(errno, 1);
    end = time(NULL);
    printf("glob = %d, time = %lds\n", glob, end - start);  // loop * 2

    glob = 0;
    start = time(NULL);
    if (pthread_create(&tid, NULL, (start_rtn)func2, NULL) != 0) err_exit(errno, 1);
    func();
    if (pthread_join(tid, NULL) != 0) err_exit(errno, 1);
    end = time(NULL);
    printf("glob = %d, time = %lds\n", glob, end - start);  // faster but may < loop * 2
}
