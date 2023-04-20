
/*
 * mutex eadlock
 */

#include "common.h"

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid;
pthread_t tid2;

const int loop = 10000;

void func() {
    for (int i = 0; i < loop; i++) {
        if (pthread_mutex_lock(&mtx) != 0) err_exit(errno, 1);
        if (pthread_mutex_lock(&mtx2) != 0) err_exit(errno, 1);

        if (pthread_mutex_unlock(&mtx2) != 0) err_exit(errno, 1);
        if (pthread_mutex_unlock(&mtx) != 0) err_exit(errno, 1);
    }
}

void func2() {
    for (int i = 0; i < loop; i++) {
        if (pthread_mutex_lock(&mtx2) != 0) err_exit(errno, 1);
        if (pthread_mutex_lock(&mtx) != 0) err_exit(errno, 1);

        if (pthread_mutex_unlock(&mtx) != 0) err_exit(errno, 1);
        if (pthread_mutex_unlock(&mtx2) != 0) err_exit(errno, 1);
    }
}

int main() {
    if (pthread_create(&tid, NULL, (start_rtn)func, NULL) != 0) err_exit(errno, 1);
    if (pthread_create(&tid2, NULL, (start_rtn)func2, NULL) != 0) err_exit(errno, 1);

    pthread_exit(NULL);
}
