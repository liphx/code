/*
 *  Condition Variables
 */

#include "common.h"

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int avail = 0;

void producer() {
    for (;;) {
        if (pthread_mutex_lock(&mtx) != 0) err_exit(errno, 1);

        avail++;
        printf("produce one, avail = %d\n", avail);

        if (pthread_mutex_unlock(&mtx) != 0) err_exit(errno, 1);

        if (avail >= 100) {
            if (pthread_cond_signal(&cond) != 0) err_exit(errno, 1);
        }
    }
}

void consumer() {
    for (;;) {
        if (pthread_mutex_lock(&mtx) != 0) err_exit(errno, 1);

        // use `while` instead of `if` when there are more consumers
        if (avail < 100) pthread_cond_wait(&cond, &mtx);

        avail -= 100;
        printf("consume one hundred, avail = %d\n", avail);

        if (pthread_mutex_unlock(&mtx) != 0) err_exit(errno, 1);
    }
}

int main() {
    pthread_t tid, tid2;
    if (pthread_create(&tid, NULL, (start_rtn)producer, NULL) != 0) err_exit(errno, 1);
    if (pthread_create(&tid2, NULL, (start_rtn)consumer, NULL) != 0) err_exit(errno, 1);
    pthread_exit(0);
}
