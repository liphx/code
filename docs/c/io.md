# 文件I/O

## 文件描述符

对内核而言，所有打开的文件都通过文件描述符引用，文件描述符是非负整数。常量`STDIN_FILENO`, `STDOUT_FILENO`, `STDERR_FILENO` 分别关联标准输入、标准输出、标准错误，定义在头文件`unistd.h`中

## 函数 `open, openat, creat, close`

```c
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags, ... /* mode_t mode */);
int openat(int dirfd, const char *pathname, int flags, ... /* mode_t mode */);
/* 若成功返回文件描述符，若失败返回-1 */

int creat(const char *pathname, mode_t mode);
/* 若成功，返回只写打开的文件描述符，若失败返回-1 */

#include <unistd.h>

int close(int fd);
/* 若成功返回0，若失败返回-1 */
```

flags 参数

| Constant      | Description |
| --            | -- |
| `O_RDONLY`    | Open the file for reading only |
| `O_WRONLY`    | Open the file for writing only |
| `O_RDWR`      | Open the file for both reading and writing |
| `O_CLOEXEC`   | Set the close-on-exec flag |
| `O_CREAT`     | Create file if it doesn’t already exist |
| `O_DIRECT`    | File I/O bypasses buffer cache |
| `O_DIRECTORY` | Fail if pathname is not a directory |
| `O_EXCL`      | With `O_CREAT`: create file exclusively |
| `O_LARGEFILE` | Used on 32-bit systems to open large files |
| `O_NOATIME`   | Don’t update file last access time on read() |
| `O_NOCTTY`    | Don’t let pathname become the controlling terminal |
| `O_NOFOLLOW`  | Don’t dereference symbolic links |
| `O_TRUNC`     | Truncate existing file to zero length |
| `O_APPEND`    | Writes are always appended to end of file |
| `O_ASYNC`     | Generate a signal when I/O is possible |
| `O_DSYNC`     | Provide synchronized I/O data integrity |
| `O_NONBLOCK`  | Open in nonblocking mode |
| `O_SYNC`      | Make file writes synchronous |

文件权限位

| Constant  | Octal value   | Permission bit    |
| --        | --            | --                |
| `S_ISUID` | 04000         | Set-user-ID       |
| `S_ISGID` | 02000         | Set-group-ID      |
| `S_ISVTX` | 01000         | Sticky            |
| `S_IRWXU` | 00700         | User-all          |
| `S_IRUSR` | 00400         | User-read         |
| `S_IWUSR` | 00200         | User-write        |
| `S_IXUSR` | 00100         | User-execute      |
| `S_IRWXG` | 00070         | Group-all         |
| `S_IRGRP` | 00040         | Group-read        |
| `S_IWGRP` | 00020         | Group-write       |
| `S_IXGRP` | 00010         | Group-execute     |
| `S_IRWXO` | 00007         | Other-all         |
| `S_IROTH` | 00004         | Other-read        |
| `S_IWOTH` | 00002         | Other-write       |
| `S_IXOTH` | 00001         | Other-execute     |

由open, openat 函数返回的文件描述符一定是最小的未用描述符数值  
`creat(pathname, mode)` 等价于 `open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode)`  
创建读写文件，可用 `open(pathname, O_RDWR | O_CREAT | O_TRUNC, mode)`  
如果文件存在且没有写权限，creat 失败，errno = EACCES, Permission denied  
pathname 是一个目录时，errno = EISDIR, Is a directory  
当要创建的文件所在的目录不存在，errno = ENOENT, No such file or directory  
关闭一个文件时会释放该进程加在该文件上的所有记录锁，当一个进程终止时，内核自动关闭它打开的所有文件。关闭一个未打开的文件描述符或关闭一个文件描述符两次都会出错。

## 函数 `lseek`

```c
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
/* 若成功，返回新的文件偏移量，若出错，返回-1 */
```

每个打开的文件关联一个“当前文件偏移量”，度量从文件开始处计算的字节数，当打开一个文件，除非指定 `O_APPEND`，该偏移量设为0

`lseek` 为打开的文件显式的设置文件偏移量， `lseek` 中 `l` 表示长整形

其中`whence`参数有三种情况  
`SEEK_SET`: 将偏移量设为距文件开始处 `offset` 个字节  
`SEEK_CUR`: 将偏移量设为当前值 + `offset` 个字节，`offset` 可正可负  
`SEEK_END`: 将偏移量设为文件长度 + `offset` 个字节，`offset` 可正可负

文件偏移量可以大于文件当前长度，这时写文件将产生空洞，该空洞并不要求占用实际的磁盘空间

## 函数 `read, write`

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
/* 返回读到的字节数， 返回0表示读到文件末尾， 若出错返回-1 */
```

从当前文件偏移量读取，在成功返回前，偏移量增加实际读到的字节数

```c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
/* 返回已写的字节数， 若出错返回-1 */
```

从当前文件偏移量写入，在成功写之后，偏移量增加实际写的字节数；指定 `O_APPEND`选项则在每次写之前将偏移量设为文件结尾处，write调用成功并不能保证数据已经写入磁盘。进程对打开的文件维护一个文件表项，含有该文件的当前偏移量（不同进程打开同一个文件拥有各自的偏移量）。任何要求多于一个函数调用的操作都不是原子操作，两个函数调用之间，内核可能会挂起进程。

使用`read`, `write`，通过 Shell 重定向复制文件

```c
char buf[4096];
int n;
while ((n = read(STDIN_FILENO, buf, 4096)) > 0) {
    if (write(STDOUT_FILENO, buf, n) != n)
        exit(1);
}

if (n < 0)
    exit(1);
```

## 函数 `pread, pwrite`

```c
#include <unistd.h>

ssize_t pread(int fd, void *buf, size_t count, off_t offset);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
// 在offset处读写，且不改变当前偏移
```

## 函数 `readv, writev`

在一次函数调用中读、写多个非连续缓冲区。

```c
#include <sys/uio.h>

ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
/* 返回读到的字节数， 返回0表示读到文件末尾， 若出错返回-1 */
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
/* 返回已写的字节数， 若出错返回-1 */

struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};

ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
```

## 函数 `dup`

```c
#include <unistd.h>
int dup(int fd);
int dup2(int fd, int fd2);
/* 若成功，返回新的文件描述符；若出错，返回-1 */

int newfd = fcntl(oldfd, F_DUPFD, startfd);
// 复制oldfd，newfd为大于等于startfd的最小未用值

// Linux
int dup3(int fd, int fd2, int flags);
int newfd = fcntl(oldfd, F_DUPFD_CLOEXEC, startfd);
// 为新描述符设置close-on-exec
```

复制一个现有的文件描述符，返回的文件描述符与`fd`共享同一个文件表项（相同的文件偏移量，不同的文件描述符标志）  
由`dup` 返回的新文件描述符一定是当前可用文件描述符中的最小数值；对于`dup2`，由`fd2`参数指定新描述符的值，如果`fd2`已经打开，则先将其关闭，如若`fd`等于`fd2`，则返回`fd2`而不关闭它

```c
int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC);
char str[2] = "01";
write(fd, str, 2);
close(fd);

fd = open("test.txt", O_RDONLY);
char a;
read(fd, &a, 1);
putc(a, stdout); // 0

int fd2 = open("test.txt", O_RDONLY);
read(fd2, &a, 1);
putc(a, stdout); // 0

int fd3 = dup(fd2);
read(fd3, &a, 1);
putc(a, stdout); // 1

int fd4 = open("test.txt", O_WRONLY | O_APPEND);
char b = '2';
write(fd4, &b, 1);

read(fd3, &a, 1);
putc(a, stdout); // 2

read(fd2, &a, 1);
putc(a, stdout); // 2, read return 0
```

## 函数 `truncate(), ftruncate()`

```c
#include <unistd.h>

int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
// 成功返回0，否则-1
```

截断文件至length字节，若文件长度小于length，将在文件末尾添加空字节或空洞文件。

## 函数 `sync, fsync, fdatasync`

```c
#include <unistd.h>
int fsync(int fd);
int fdatasync(int fd);
/* 若成功返回0；若出错返回-1 */
void sync(void);
```

`sync` 将所有修改过的块缓冲区排入写队列，然后就返回，它并不等待实际写磁盘操作结束  
`fsync` 只对`fd`指定的文件起作用，并等待实际写磁盘操作结束再返回  
`fdatasync` 类似`fsync`，但只影响数据部分，`fsync`会同步更新文件的属性

## 函数 `fcntl`

```c
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* int arg */);
/* 若成功，返回值依赖cmd，若失败，返回-1 */
```

用来改变已经打开文件的属性。

```c
int flags = fcntl(fd, F_GETFL); // 返回已打开的文件属性
int ret = fcntl(fd, F_SETFL, flags /* int */);  // 设置属性
```

## 文件 `/dev/fd/n`

打开文件`/dev/fd/n` 等效于复制描述符`n`（假定描述符`n`已打开）  
`/dev/fd/stdin`, `/dev/fd/stdout`, `/dev/fd/stderr`等效于`/dev/fd/0`, `/dev/fd/1`, `/dev/fd/2`

## 函数 `stat`

```c
#include <sys/stat.h>
int stat(const char *restrict pathname, struct stat *restrict buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *restrict pathname, struct stat *restrict buf);
int fstatat(int fd, const char *restrict pathname, struct stat *restrict buf, int flag);
/* 若成功均返回0，若出错返回-1 */
```

`stat`函数通过参数`pathname`，给出文件相关的信息结构；`fstat`是通过文件描述符得到；`lstat`类似`stat`，但当文件是符号链接时，`stat`给出链接引用的文件的信息，`lstat`给出该链接本身的信息；`fstatat`相对于参数`fd`指向的目录给出文件信息，当`flag`设置为`AT_SYMLINK_NOFOLLOW`，该函数不跟随符号链接，当`fd`参数是`AT_FDCWD`时，在当前目录计算，但如果路径名是绝对路径则忽略`fd`参数

`struct stat`基本形式

```c
struct stat {
    mode_t      st_mode;    /* file type & mode */
    ino_t       st_ino;     /* i-node number */
    dev_t       st_dev;     /* device number */
    nlink_t     st_nlink;   /* number of links */
    uid_t       st_uid;     /* user id of owner */
    gid_t       st_gid;     /* group if of owner */
    off_t       st_size;    /* size in bytes, for regular files */
    /* ... */
}
```

示例

```c
void filetype(mode_t mode, char buf[]) {
    if (S_ISREG(mode))
        strcpy(buf, "regular file");
    else if(S_ISDIR(mode))
        strcpy(buf, "directory file");
    else if(S_ISCHR(mode))
        strcpy(buf, "character special file");
    else if(S_ISBLK(mode))
        strcpy(buf, "block special file");
    else if(S_ISFIFO(mode))
        strcpy(buf, "pipe or FIFO");
    else if(S_ISLNK(mode))
        strcpy(buf, "symbolic link");
    else if(S_ISSOCK(mode))
        strcpy(buf, "socket");
    else
        strcpy(buf, "unknow file type");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: stat pathname\n");
        exit(1);
    }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("stat error");
        exit(1);
    }

    printf("file %s stat:\n", argv[1]);

    char typebuf[20];
    filetype(st.st_mode, typebuf);
    printf("    st_mode = %u, %s\n", st.st_mode, typebuf);

    printf("    st_ino = %ld\n", st.st_ino);
    printf("    st_dev = %ld\n", st.st_dev);
    printf("    st_nlink = %ld\n", st.st_nlink);
    printf("    st_uid = %u\n", st.st_uid);
    printf("    st_gid = %u\n", st.st_gid);
    printf("    st_size = %ld\n", st.st_size);
    printf("    st_blocks = %ld\n", st.st_blocks);

    char timebuf[20];

    // 最近访问时间
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_atime));
    printf("    st_atime = %ld, localtime is %s\n", st.st_atime, timebuf);

    // 最近修改时间(文件内容)
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_mtime));
    printf("    st_mtime = %ld, localtime is %s\n", st.st_mtime, timebuf);

    // 最近变化时间(内容，权限，所有者等)
    (void)strftime(timebuf, sizeof(timebuf), "%F %T", localtime(&st.st_ctime));
    printf("    st_ctime = %ld, localtime is %s\n", st.st_ctime, timebuf);
}
```

## 文件类型

(1) 普通文件 (regular file) 包含某种形式的数据，不区分文本文件和二进制文件
(2) 目录文件 (directory file) 包含其它文件的名字及指向相关文件信息的指针，只有内核可以直接写目录
(3) 块特殊文件 (block special file) 提供对设备带缓冲的访问
(4) 字符特殊文件 (character special file) 提供对设备不带缓冲的访问
(5) FIFO 用于进程间通信，也称为命名管道
(6) 套接字 (socket) 进程间的网络通信
(7) 符号链接 (symbolic link) 指向另一个文件

用于确定文件类型的宏

```c
S_ISREG()   /* 是否是普通文件 */
S_ISDIR()   /* 是否是目录 */
S_ISCHR()   /* 是否是字符特殊文件 */
S_ISBLK()   /* 是否是块特殊文件 */
S_ISFIFO()  /* 是否是FIFO */
S_ISLNK()  /* 是否是符号链接 */
S_ISSOCK()  /* 是否是套接字 */
/* 参数均为struct stat中的st_mode成员 */
```

## 文件访问权限

1. 拥有对目录的执行权限时才能打开目录内的文件
2. 对目录拥有读权限可以获得在该目录中所有文件名的列表
3. 拥有读/写权限才能打开现有文件进行读/写操作
4. 在目录中创建一个文件，必须拥有这个目录的写权限和执行权限
5. 删除现有文件，必须对这个文件所在目录具有写权限和执行权限，对该文件不需要有读、写权限
6. `exec`函数执行一个文件，必须有执行权限，且只能是普通文件

## 函数 `access`

```c
#include <unistd.h>
int access(const char *pathname, int mode);
int faccessat(int fd, const char *pathname, int mode, int flag);
/* 若成功返回0；若出错返回-1 */
```

按实际用户ID和实际组ID进行访问权限测试

其中`mode`参数

`F_OK`测试文件是否已经存在  
`R_OK`测试读权限  
`W_OK`测试写权限
`X_OK`测试执行权限

`faccessat` 中`flag`参数设为`AT_EACCESS`时，访问检查用的是调用进程的有效用户ID和有效组ID

## 函数 `umask`

```c
#include <sys/stat.h>
mode_t umask(mode_t cmask);
/* 返回之前的文件模式创建屏蔽字 */
```

## 标准库`<stdio>`

```c
typedef /* unspecified */ FILE;
FILE *fopen(const char *filename, const char *mode);
int fflush(FILE *stream);
size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
```

## inotify

```c
#include <sys/inotify.h>
int inotify_init(void);
// 成功返回文件描述符，否则-1
int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
// 成功返回watch文件描述符，否则-1
int inotify_rm_watch(int fd, int wd);
// 成功返回0，否则-1

struct inotify_event {
    int      wd;       /* Watch descriptor */
    uint32_t mask;     /* Mask describing event */
    uint32_t cookie;   /* Unique cookie associating related events (for rename(2)) */
    uint32_t len;      /* Size of name field */
    char     name[];   /* Optional null-terminated name */
};
```
