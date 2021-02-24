# POSIX Thread

线程标识

`pthread_t`, 为了程序可移植，需用函数`pthread_equal`来比较两个线程ID，调用`pthread_self`获得自身线程ID, Linux 下以 `unsigned long` 表示

函数汇总

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
```
