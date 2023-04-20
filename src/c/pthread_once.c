#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_once_t once_control = PTHREAD_ONCE_INIT;
void init() { printf("init in %d\n", pthread_self()); }

void err_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void fn() {
    printf("call fn in %d\n", pthread_self());
    if (pthread_once(&once_control, init) != 0) err_exit("pthread_once");
}

int main() {
    pthread_t t;
    if (pthread_create(&t, NULL, fn, NULL) != 0) err_exit("pthread_create");
    fn();
    pthread_join(t, NULL);
}
