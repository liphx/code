#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static inline void err_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

pthread_key_t key;
int loop = 1000 * 1000;

void dest(void *value) {
    printf("dest: %d\n", *(int *)value);
    free(value);
}

void *fn(void *) {
    for (int i = 0; i < loop; i++) {
        int *data = pthread_getspecific(key);
        if (data == NULL) {
            data = (int *)malloc(sizeof(int));
            if (data == NULL) err_exit("malloc");
            if (pthread_setspecific(key, data) != 0) err_exit("pthread_setspecific");
        }

        ++*data;
        if (*data % 1000 == 0) printf("%ld: %d\n", pthread_self(), *data);
    }

    return NULL;
}

int main() {
    if (pthread_key_create(&key, dest) != 0) err_exit("pthread_create_key");

    pthread_t tid;
    if (pthread_create(&tid, NULL, fn, NULL) != 0) err_exit("pthread_create");
    pthread_t tid2;
    if (pthread_create(&tid2, NULL, fn, NULL) != 0) err_exit("pthread_create");

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
}
