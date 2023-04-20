#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define err_exit(msg, exit_code) \
    {                            \
        perror(msg);             \
        exit(exit_code);         \
    }

typedef void *(*start_rtn)(void *);

int global = 0;

void thread_strat() {
    for (int i = 0; i < 100000; i++) {
        global++;
    }
    printf("new thread global = %d\n", global);
    pthread_detach(pthread_self());
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    if (pthread_create(&tid, NULL, (start_rtn)thread_strat, NULL) != 0) {
        err_exit("pthread_create fail", 1);
    }

    for (int i = 0; i < 100000; i++) {
        global++;
    }
    printf("main thread global = %d\n", global);

    pthread_exit(NULL);
}
