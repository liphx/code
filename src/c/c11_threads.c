#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_NO_THREADS__
#include <threads.h>
#endif

#define err_exit(msg, exit_code) \
    {                            \
        perror(msg);             \
        exit(exit_code);         \
    }

int newthrd_start(void *arg) {
    printf("newthrd start, tid = %lu\n", thrd_current());
    return 0;
}

void newthrd2_start(void *arg) {
    printf("newthrd2 start, tid = %lu\n", thrd_current());
    thrd_exit(0);
}

// 编译时需链接 pthread
int main(int argc, char *argv[]) {
#ifdef __STDC_NO_THREADS__
    puts("threads not support");
    exit(1);
#endif

    thrd_t tid;
    if (thrd_create(&tid, newthrd_start, NULL) != thrd_success) {
        err_exit("thrd_create fail", 1);
    }

    if (thrd_join(tid, NULL) != thrd_success) {
        err_exit("thrd_join fail", 1);
    }

    thrd_t tid2;
    if (thrd_create(&tid2, (thrd_start_t)newthrd2_start, NULL) != thrd_success) {
        err_exit("thrd_create fail", 1);
    }

    thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);  // 主线程休眠1s
    if (thrd_detach(tid2) != thrd_success) {
        err_exit("thrd_detach fail", 1);
    }

    thrd_exit(0);
}
