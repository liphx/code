#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define err_exit(msg, exit_code) \
    {                            \
        perror(msg);             \
        exit(exit_code);         \
    }

#define thread_printf(fmt, args...)             \
    {                                           \
        printf("thread %lu: ", pthread_self()); \
        printf(fmt, ##args);                    \
    }

typedef void *(*start_rtn)(void *);

void thread_strat() { thread_printf("new thread\n"); }

int *thread2_strat(int *arg) {
    thread_printf("new thread2\n");
    int *ret = (int *)malloc(sizeof(int));
    *ret = *arg + 1;
    return ret;  // or pthread_exit(ret);
}

void thread3_strat(int *arg) { thread_printf("new thread3\n"); }

int main(int argc, char *argv[]) {
    thread_printf("main thread\n");

    pthread_t tid;
    if (pthread_create(&tid, NULL, (start_rtn)thread_strat, NULL) != 0) {
        err_exit("pthread_create fail", 1);
    }

    if (pthread_join(tid, NULL) != 0) {
        err_exit("pthread_join fail", 1);
    }

    pthread_t tid2;
    int arg = 100, *ret;
    if (pthread_create(&tid2, NULL, (start_rtn)thread2_strat, &arg) != 0) {
        err_exit("pthread_create fail", 1);
    }

    if (pthread_join(tid2, (void **)&ret) != 0) {
        err_exit("pthread_join fail", 1);
    }

    printf("thread2 return %d\n", *ret);  //=> 101
    free(ret);

    pthread_t tid3;
    if (pthread_create(&tid3, NULL, (start_rtn)thread3_strat, NULL) != 0) {
        err_exit("pthread_create fail", 1);
    }
    if (pthread_detach(tid3) != 0) {
        err_exit("pthread_detach fail", 1);
    }

    pthread_exit(NULL);
}
