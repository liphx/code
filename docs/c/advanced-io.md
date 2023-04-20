# 高级 I/O

## 非阻塞 I/O

将文件描述符设为非阻塞I/O 的方法

* `open` 时指定 `O_NONBLOCK`
* 对打开的文件描述符调用 `fcntl`, 设置 `O_NONBLOCK`

如果是非阻塞I/O， `open, read, write` 不能完成时，出错返回

```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define IS_FD_NONBLOCK(fd) (fcntl(fd, F_GETFL) & O_NONBLOCK)

int set_fl(int fd, int flags) {
    int prev = fcntl(fd, F_GETFL);
    if (prev == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, prev | flags);
}

int clr_fl(int fd, int flags) {
    int prev = fcntl(fd, F_GETFL);
    if (prev == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, prev & ~flags);
}

int main(int argc, char *argv[]) {
    set_fl(STDIN_FILENO, O_NONBLOCK);
    char buf[BUFSIZ];
    int ret = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "ret = %d, errno = %d: %s\n", ret, errno, strerror(errno));
    // => ret = -1, errno = 11: Resource temporarily unavailable
    // error: EAGAIN
    clr_fl(STDIN_FILENO, O_NONBLOCK);
    return 0;
}
```

## 记录锁（字节范围锁）

```c
#include <fcntl.h>
int fcntl(int fd, int cmd, .../* args*/);

// cmd: F_GETLK, F_SETLK, F_SETLKW(F_SETLK Wait 版本)
// args: struct flock *

struct flock {
    short l_type; // F_RDLCK(共享读锁), F_WRLCK(独占性写锁), F_UNLCK(解锁一个区域)
    short l_whence; // SEEK_SET, SEEK_CUR, SEEK_END
    off_t l_start; // 相对于l_whence 的起始偏移量
    off_t l_len; // 字节长度
    pid_t l_pid; // 仅由F_GETLK返回， l_pid 持有的锁能阻塞当前进程
}
```

* 锁可以在当前文件尾端或越过文件尾端出开始，但不能在文件的起始位置之前开始
* l_len = 0, 锁的范围可以扩大到最大可能的偏移量
* l_whence = SEEK_SET, l_start = 0, l_len = 0, 则对整个文件加锁
* 加读锁时，描述符必须是读打开；加写锁时，描述符必须是写打开

多个进程在当前区域(某个给定的字节)加锁的规则

* 无锁时，可以请求加读锁或写锁
* 有读锁时，可以请求读锁，拒绝请求写锁
* 有写锁时，请求锁均拒绝

持有锁的进程本身再请求锁，新锁会替换旧锁

F_GETLK 测试能否建立一把锁， F_SETLK 企图建立锁，但这两者不是一个原子操作

锁的自动释放

* 持有锁的进程退出
* `close(fd);`

锁的继承

* `fork` 产生的子进程不会继承父进程的锁
* `exec` 新程序继承原执行程序的锁（如果文件描述符设置了执行时关闭标志，则锁会被释放）

建议性锁仅在合作进程中有效，不能阻止其他进程写一个有写权限的文件；强制性锁则会让内核检查每一个`open, read, write`。`Linux` 两者都支持

```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}

/* 无法测试进程自己是否对某个文件加锁 */
pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    if (fcntl(fd, F_GETLK, &lock) < 0) {
        fprintf(stderr, "fcntl error\n");
        return -1;
    }

    if (lock.l_type == F_UNLCK)
        return 0;
    return lock.l_pid;
}

#define read_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

#define is_read_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

#define lockfile(fd) write_lock((fd), 0, SEEK_SET, 0)
```

## I/O 多路转接

函数 `select` 与 `pselect`

```c
#include <sys/select.h>

int select(int maxfdp1, fd_set *restrict readfds,
    fd_set *restrict writefds, fd_set *restrict exceptfds,
    struct timeval *restrict tvptr);
// maxfdp1: max fd plus 1, 在3个fd_set 中找出最大描述符并加1，也可以设置为 FD_SETSIZE
// readfds, writefds, exceptfds: 我们关心的可读、可写、处于异常条件的描述符集合
// tvptr == NULL, 永远等待；tvptr−>tv_sec == 0 && tvptr−>tv_usec == 0, 不等待；tvptr−>tv_sec !=0 || tvptr−>tv_usec != 0, 等待指定的时间
// 返回就绪的文件描述符的数量；若超时，返回0；若出错，返回-1

int FD_ISSET(int fd, fd_set *fdset);
// 判断fd 是否在fd_set
void FD_CLR(int fd, fd_set *fdset);
void FD_SET(int fd, fd_set *fdset);
void FD_ZERO(fd_set *fdset);
// 声明fd_set 后，必须调用FD_ZERO 置0

// POSIX select
int pselect(int maxfdp1, fd_set *restrict readfds,
    fd_set *restrict writefds, fd_set *restrict exceptfds,
    const struct timespec *restrict tsptr,
    const sigset_t *restrict sigmask);
```

使用

`main.c`

```c
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(STDIN_FILENO, &readset);

    char buf[BUFSIZ];
    struct timeval tm;
    tm.tv_sec = 2;

    while (1) {
        // Linux 会用剩余时间更新 tm
        int num = select(1, &readset, NULL, NULL, &tm);
        if (num == 0) break;
        fprintf(stderr, "num = %d\n", num);
        for (int i = 0; i < num; i++) {
            int len = read(STDIN_FILENO, buf, sizeof(buf));
            if (len == 0)
                FD_CLR(STDIN_FILENO, &readset);
            fprintf(stderr, "read %d bytes\n", len);
            fprintf(stderr, "%s", buf);
        }

    }

    return 0;
}
```

```shell
$ gcc main.c
$ for i in {1..5}; do echo abc$i; sleep 1; done | ./a.out
num = 1
read 5 bytes
abc1
num = 1
read 5 bytes
abc2
```

函数 `poll`

```c
#include <poll.h>
int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);
// nfds: fdarray 中元素个数
// timeout = -1, 永远等待；timeout = 0, 不等待；timeout > 0, 等待 timeout 毫秒
// 返回就绪的文件描述符的数量；若超时，返回0；若出错，返回-1

struct pollfd {
    int    fd;
    short  events; // 告诉内核关心的事件
    short  revents; // 函数返回时说明描述符上发生了哪些事件
};
```

events 包括

| Name          | events, revents?  | 说明                                  |
| :--           | :---:             | :---                                  |
| POLLIN        |  events, revents  | 可以不阻塞地读高优先级数据以外的数据  |
| POLLRDNORM    |  events, revents  | 可以不阻塞地读普通数据                |
| POLLRDBAND    |  events, revents  | 可以不阻塞地读优先级数据              |
| POLLPRI       |  events, revents  | 可以不阻塞地读高优先级数据            |
| POLLOUT       |  events, revents  | 可以不阻塞地写普通数据                |
| POLLWRNORM    |  events, revents  | 同 POLLOUT                            |
| POLLWRBAND    |  events, revents  | 可以不阻塞地写优先级数据              |
| POLLERR       |  revents          | 已出错                                |
| POLLHUP       |  revents          | 已挂断                                |
| POLLNVAL      |  revents          | 描述符没有引用一个打开文件            |

## 异步 I/O

POSIX 异步 I/O

```c
#include <aio.h>

int aio_read(struct aiocb *aiocb);
int aio_write(struct aiocb *aiocb);
// 若成功返回0；失败返回-1
// 返回成功时，异步I/O 请求已经加入操作系统等待处理的队列中了，和实际I/O 操作的结果无关

int aio_fsync(int op, struct aiocb *aiocb);
// op: O_DSYNC(类似fdatasync), O_SYNC(类似fsync)
// 若成功返回0；失败返回-1
// 同前两个函数，返回时如果同步操作完成前数据不会持久话

int aio_error(const struct aiocb *aiocb);
// 获取异步读写或同步操作的结果，返回值：
// 0, 异步操作成功，需要调用aio_return 获取操作返回值
// -1, aio_error失败，可查看errno
// EINPROGRESS, 异步操作仍在等待
// else, 异步操作失败返回的错误码

ssize_t aio_return(const struct aiocb *aiocb);
// aio_error 返回0 后再调用，否则结果是未定义的
// 对每个异步操作仅调用一次
// 函数调用失败返回-1，否则返回异步操作的结果(read, write, fsync返回的结果)

struct aiocb {
    int aio_fildes;                 /* file descriptor */
    off_t aio_offset;               /* file offset for I/O */
    volatile void  *aio_buf;        /* buffer for I/O */
    size_t aio_nbytes;              /* number of bytes to transfer */
    int aio_reqprio;                /* priority */
    struct sigevent aio_sigevent;   /* signal information */
    int aio_lio_opcode;             /* operation for list I/O */
};

struct sigevent {
    int sigev_notify;               /* notify type: SIGEV_NONE, SIGEV_SIGNAL, SIGEV_THREAD */
    int sigev_signo;                /* signal number */
    union sigval sigev_value;       /* notify argument */
    void (*sigev_notify_function)(union sigval); /* notify function */
    pthread_attr_t *sigev_notify_attributes;     /* notify attrs */
};

int aio_suspend(const struct aiocb *constlist[], int nent,
    const struct timespec *timeout);
// 成功返回0，否则返回-1

int aio_cancel(int fd, struct aiocb *aiocb);
// 返回值：
// AIO_ALLDONE      所有操作在尝试取消前已经完成
// AIO_CANCELED     所有要求的操作已被取消
// AIO_NOTCANCELED  至少有一个要求的操作没有被取消
// -1               函数调用失败，记录errno

int lio_listio(int mode, struct aiocb *restrict const list[restrict],
    int nent, struct sigevent *restrict sigev);
// 成功返回0，否则返回-1
```

## 函数 `readn, writen`

读写指定的n 字节数据

```c
ssize_t             /* Read "n" bytes from a descriptor  */
readn(int fd, void *ptr, size_t n)
{
    size_t      nleft;
    ssize_t     nread;

    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return(-1); /* error, return -1 */
            else
                break;      /* error, return amount read so far */
        } else if (nread == 0) {
            break;          /* EOF */
        }
        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);      /* return >= 0 */
}

ssize_t             /* Write "n" bytes to a descriptor  */
writen(int fd, const void *ptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return(-1); /* error, return -1 */
            else
                break;      /* error, return amount written so far */
        } else if (nwritten == 0) {
            break;
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n - nleft);      /* return >= 0 */
}

// 成功返回已读或已写的字节数，失败返回-1
// 若已经读、写了一些数据后出错，返回的是已传输的数据量
```

## 存储映射 I/O

将磁盘文件映射到存储空间的缓冲区上，读缓冲区相当于读磁盘，写缓冲区相当于写磁盘

```c
#include <sys/mman.h>
void *mmap(void *addr, size_t len, int prot, int flag, int fd, off_t off);
// 成功返回映射区的起始地址，出错返回 MAP_FAILED
```

参数

* addr, 指定映射存储区的起始地址，设为0表示由系统选择
* fd, 打开的文件描述符
* len, 映射的字节数
* off, 起始偏移量
* port, 以下参数或任意组合的按位或, 受限于打开文件自身的权限
    * PROT_READ, 映射区可读
    * PROT_WRITE, 映射区可写
    * PROT_EXEC, 映射区可执行
    * PROT_NONE, 映射区不可访问
* flag, MAP_SHARED 和 MAP_PRIVATE 之一必须被指定
    * MAP_FIXED, 返回值必须等于addr
    * MAP_SHARED, 指定存储映射操作修改映射文件
    * MAP_PRIVATE, 指定存储映射操作导致创建文件的一个私有副本，后面对该映射区的引用都是引用副本部分

子进程通过`fork` 可以继承存储映射区域，新程序不能通过`exec` 继承

```c
#include <sys/mman.h>

int mprotect(void *addr, size_t len, int prot);
// 更改现有映射，addr 必须是系统页长的整数倍
// 若成功返回0，否则返回-1

int msync(void *addr, size_t len, int flags);
// 冲洗到被映射的文件中，flags: MS_ASYNC, MS_SYNC, MS_INVALIDATE
// 若成功返回0，否则返回-1

int munmap(void *addr, size_t len);
// 解除映射(进程退出会自动解除映射，关闭文件描述符不会)
// 若成功返回0，否则返回-1
```
