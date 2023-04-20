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

void hello(void *msg) { printf("hello %s\n", (const char *)msg); }

void thread_strat() {
    pthread_cleanup_push(hello, "abc");
    pthread_cleanup_push(hello, "123");  // => 先被调用
    pthread_exit(NULL);
    pthread_cleanup_pop(0);  // for compiler
    pthread_cleanup_pop(0);
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    if (pthread_create(&tid, NULL, (start_rtn)thread_strat, NULL) != 0) {
        err_exit("pthread_create fail", 1);
    }

    if (pthread_join(tid, NULL) != 0) {
        err_exit("pthread_join fail", 1);
    }

    pthread_exit(NULL);
}
