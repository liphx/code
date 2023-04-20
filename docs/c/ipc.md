# 进程间通信

## 管道

管道是半双工（一端读一端写）的，且只能在父子进程间使用

```c
#include <unistd.h>

int pipe(int fd[2]);
// 成功返回0，否则返回-1
```

经由参数fd 返回两个文件描述符，fd[0] 为读而打开，fd[1] 为写打开。fd[1] 的输出是fd[0] 的输入。(管道在实现实现上可以作为双全工，两个文件描述符都以读写方式打开。)

对返回的fd 调用`fstat` 会返回FIFO类型，可用`S_ISFIFO` 测试

`fork` 之后，如果想把数据从父进程传给子进程，则父进程关闭读端(fd[0]), 子进程关闭写端(fd[1]); 反之父进程关闭fd[1], 子进程关闭fd[0]

```c
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd[2];
    int ret = pipe(fd);
    struct stat st;
    fstat(fd[0], &st);
    printf("S_ISFIFO is %d\n", S_ISFIFO(st.st_mode));

    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid == 0) { // child
        close(fd[0]);
        char buf[] = "this is child process";
        write(fd[1], buf, sizeof(buf));
    } else { // parent
        wait(NULL);
        close(fd[1]);
        char buf[BUFSIZ];
        read(fd[0], buf, sizeof(buf));
        printf("%s\n", buf);
    }

    exit(0);
}
```

输出

```
S_ISFIFO is 1
this is child process
```

## 函数 `popen, pclose`

```c
#include <stdio.h>

FILE *popen(const char *cmdstring, const char *type);
// 成功返回FILE *，否则为NULL
int pclose(FILE *fp);
// 成功返回cmdstring 终止状态，否则返回-1
```

popen 先fork, 然后调用exec 执行cmdstring, 并返回FILE *. 如果type 是"r", 则FILE * 连接到执行cmdstring 的标准输出; 如果type 是"w", 则FILE * 连接到执行cmdstring 的标准输入, 即"r" 表示可读, "w" 可写

cmdstring 将以`sh -c cmdstring` 执行

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp = popen("echo this is child process", "r");
    if (fp == NULL) exit(1);
    char buf[BUFSIZ];
    fgets(buf, BUFSIZ, fp);
    printf("%s", buf);
    fclose(fp);
    exit(0);
}
```

## 协同进程

当一个进程的标准输入输出都连接到管道的时候，就成了协同进程

## FIFO

未命名的管道只能在两个相关的进程间使用，且这两个进程还要有一个共同的祖先进程。FIFO(命名管道)可以使不相关的进程交换数据

```c
#include <sys/stat.h>
int mkfifo(const char *path, mode_t mode);
int mkfifoat(int fd, const char *path, mode_t mode);
// 成功返回0，否则返回-1
```

## XSI IPC

```c
#include <sys/ipc.h>

key_t ftok(const char *path, int id);
// 成功返回key，否则返回-1

struct ipc_perm {
    uid_t  uid;  /* owner’s effective user ID */
    gid_t  gid;  /* owner’s effective group ID */
    uid_t  cuid; /* creator’s effective user ID */
    gid_t  cgid; /* creator’s effective group ID */
    mode_t mode; /* access modes */
    ...
};
```

Linux 使用`ipcs -l` 查看XSI IPC的内置限制

```shell
$ ipcs -l

------ Messages Limits --------
max queues system wide = 32000
max size of message (bytes) = 8192
default max size of queue (bytes) = 16384

------ Shared Memory Limits --------
max number of segments = 4096
max seg size (kbytes) = 18014398509465599
max total shared memory (kbytes) = 18014398509481980
min seg size (bytes) = 1

------ Semaphore Limits --------
max number of arrays = 32000
max semaphores per array = 32000
max semaphores system wide = 1024000000
max ops per semop call = 500
semaphore max value = 32767

```

## 消息队列

```c
#include <sys/msg.h>
int msgget(key_t key, int flag);
// 成功返回消息队列ID，否则返回-1

int msgctl(int msqid, int cmd, struct msqid_ds *buf);
// 成功返回0，否则返回-1

int msgsnd(int msqid, const void *ptr, size_t nbytes, int flag);
// 成功返回0，否则返回-1

ssize_t msgrcv(int msqid, void *ptr, size_t nbytes, long type, int flag);
// 若成功返回消息数据部分的长度，否则返回-1
```

## 信号量

```c
#include <sys/sem.h>
int semget(key_t key,int nsems, int flag);
// 成功返回信号量ID，否则返回-1

int semctl(int semid, int semnum, int cmd, ... /* union semunarg*/ );

int semop(int semid, struct sembuf semoparray[], size_t nops);
// 成功返回0，否则返回-1
```

## 共享内存

```c
#include <sys/shm.h>

int shmget(key_t key, size_t size, int flag);
// 成功返回共享ID，否则返回-1

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
// 成功返回0，否则返回-1

void *shmat(int shmid, const void *addr, int flag);
// 若成功返回共享内存地址

int shmdt(const void *addr);
// 成功返回0，否则返回-1
```