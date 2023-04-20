# POSIX Thread

## 线程标识

`pthread_t`, 为了程序可移植，需用函数`pthread_equal`来比较两个线程ID，调用`pthread_self`获得自身线程ID, Linux 下以 `unsigned long` 表示

## 函数汇总

```c
#include <pthread.h>
int pthread_equal(pthread_t tid1, pthread_t tid2);
// 若相等返回非0数值，否则返回0

pthread_t pthread_self(void);
// 返回调用线程的线程ID

int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
// 创建线程，函数成功返回时，新创建的线程ID会被设置为tidp指向的内存单元
// attr参数用于定制不同的线程属性，设为NULL时为默认属性
// 新创建的线程从start_rtn函数的地址处开始运行，arg为运行函数的参数地址
// 若成功返回0，若出错返回出错编号

void pthread_exit(void *rval_ptr);
// 线程退出

int pthread_join(pthread_t thread, void **rval_ptr);
// 调用线程阻塞直到指定线程退出或取消；成功返回0；失败返回错误码

int pthread_cancel(pthread_t tid);
// 请求取消线程，若成功返回0，否则返回非0

void pthread_cleanup_push(void (*rtn)(void *), void *arg);
void pthread_cleanup_pop(int execute);
// 线程处理清理函数

int pthread_detach(pthread_t tid);
// 分离线程，若成功返回0，否则返回非0

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// 加/解锁互斥量，成功返回0，失败返回错误码
// lock 时如果互斥量已被锁定，阻塞调用者
// unlock 被其它线程加锁或未被加锁的互斥量返回错误

int pthread_mutex_trylock(pthread_mutex_t *mutex);
// 如果信号量已被加锁，返回 EBUSY，其它同 lock

#include <time.h>
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
    const struct timespec *restrict abstime);

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
// 成功返回0，失败返回错误码
// 初始化/销毁互斥量

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
// 静态分配的互斥量

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
// 成功返回0，失败返回错误码

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// 静态分配的条件变量

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
// 成功返回0，失败返回错误码
// 初始化/销毁条件变量
```
