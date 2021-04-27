# 文件I/O

## open, openat, creat, close

```c
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int openat(int dirfd, const char *pathname, int flags);
int openat(int dirfd, const char *pathname, int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);
/* Returns file descriptor on success, or –1 on error */

#include <unistd.h>

int close(int fd);
/* Returns 0 on success, or –1 on error */
```

flags 参数

| Constant      | Description |
| --            | -- |
| O_RDONLY      | Open the file for reading only |
| O_WRONLY      | Open the file for writing only |
| O_RDWR        | Open the file for both reading and writing |
| O_CLOEXEC     | Set the close-on-exec flag |
| O_CREAT       | Create file if it doesn’t already exist |
| O_DIRECT      | File I/O bypasses buffer cache |
| O_DIRECTORY   | Fail if pathname is not a directory |
| O_EXCL        | With O_CREAT: create file exclusively |
| O_LARGEFILE   | Used on 32-bit systems to open large files |
| O_NOATIME     | Don’t update file last access time on read() |
| O_NOCTTY      | Don’t let pathname become the controlling terminal |
| O_NOFOLLOW    | Don’t dereference symbolic links |
| O_TRUNC       | Truncate existing file to zero length |
| O_APPEND      | Writes are always appended to end of file |
| O_ASYNC       | Generate a signal when I/O is possible |
| O_DSYNC       | Provide synchronized I/O data integrity |
| O_NONBLOCK    | Open in nonblocking mode |
| O_SYNC        | Make file writes synchronous |

文件权限位

| Constant  | Octal value   | Permission bit    |
| --        | --            | --                |
| S_ISUID   | 04000         | Set-user-ID       |
| S_ISGID   | 02000         | Set-group-ID      |
| S_ISVTX   | 01000         | Sticky            |
| S_IRWXU   | 00700         | User-all          |
| S_IRUSR   | 00400         | User-read         |
| S_IWUSR   | 00200         | User-write        |
| S_IXUSR   | 00100         | User-execute      |
| S_IRWXG   | 00070         | Group-all         |
| S_IRGRP   | 00040         | Group-read        |
| S_IWGRP   | 00020         | Group-write       |
| S_IXGRP   | 00010         | Group-execute     |
| S_IRWXO   | 00007         | Other-all         |
| S_IROTH   | 00004         | Other-read        |
| S_IWOTH   | 00002         | Other-write       |
| S_IXOTH   | 00001         | Other-execute     |

* 由open, openat 函数返回的文件描述符一定是最小的未用描述符数值
* `creat(pathname, mode)` 等价于 `open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode)`
* 创建读写文件，可用 `open(pathname, O_RDWR | O_CREAT | O_TRUNC, mode)`
* 如果文件存在且没有写权限，creat 失败，errno = EACCES, Permission denied
* pathname 是一个目录时，errno = EISDIR, Is a directory
* 当要创建的文件所在的目录不存在，errno = ENOENT, No such file or directory
