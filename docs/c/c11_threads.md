# threads

c11起，标准库定义了线程支持库 `<threads.h>`，若编译器定义了 `__STDC_NO_THREADS__`， 则编译器暂不支持

## 类型

* `thrd_t` 线程标识
* `thrd_start_t` 创建线程开始执行的函数，`int(*)(void*)` 的 `typedef`
* 线程状态

```c
enum {
    thrd_success,   // 成功
    thrd_nomem,     // 内存耗尽
    thrd_timedout,  // 时间用尽
    thrd_busy,      // 资源暂时不可用
    thrd_error      // 不成功
};
```

## 函数

```c
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
// 若成功返回  thrd_success; 若内存不足返回 thrd_nomem; 若出现其他错误返回 thrd_error
// 从函数 func 返回等价于以等于 func 返回值的参数调用 thrd_exit

void thrd_exit(int res);
// 线程退出

thrd_t thrd_current(void);
// 返回调用方线程的标识符

int thrd_equal(thrd_t lhs, thrd_t rhs);
// 若 lhs 与 rhs 表示同一线程则为非零值，否则为 0

int thrd_join(thrd_t thr, int *res);
// 阻塞当前线程，直到 thr 所标识的线程完成执行
// 若 res 不是空指针，则将该线程的结果码放置到 res 所指向的位置
// 若成功返回 thrd_success; 否则返回 thrd_error

int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
// 阻塞当前线程的执行，至少直至经过 duration 所指向的基于 TIME_UTC 的时长
// 若收到不忽略的信号（ signal ），则可以较早地从休眠恢复，。此情况下，若 remaining 非 NULL ，则存储剩余时长到 remaining 所指向的对象中
// duration 和 remaining 可以指向相同对象
// 成功休眠时为0，若出现信号则为-1，若出现错误则为其他负值

void thrd_yield(void);
// 向实现提供一个重新安排线程执行的提示，允许其他线程运行

int thrd_detach(thrd_t thr);
// 将 thr 所标识的线程从当前环境中分离。一旦该线程退出，就自动释放其保有的资源
// 若成功返回 thrd_success; 否则返回 thrd_error
```
