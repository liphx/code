# thread-local storage

```c
#include <pthread.h>

pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
// ensure init_routine be called once
// *once_control must be statically initialized with PTHREAD_ONCE_INIT
// returns 0 on success, otherwise errno

int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
// key should point to a global variable
// destructor called when thread exit and value is non-NULL with the key
// returns 0 on success, otherwise errno

int pthread_setspecific(pthread_key_t key, const void *value);
// returns 0 on success, otherwise errno

void *pthread_getspecific(pthread_key_t key);
// return value with the key, or NULL
```
