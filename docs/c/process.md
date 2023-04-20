# 进程

## 进程环境

### `_exit, _Exit, exit`

 三个函数用于正常终止一个程序：`_exit`, `_Exit` 立即进入内核，`exit`先执行一些清理再返回内核

```c
#include <stdlib.h>
void exit(int status);
void _Exit(int status);
#include <unistd.h>
void _exit(int status);
```

执行`exit`会对所有打开流调用`close`，在`main`函数中，`return` 与 `exit`函数作用相同

在shell中输入`echo $?`可以查看上一个进程的退出状态

### `atexit`

登记终止处理函数

```c
#include <stdlib.h>
int atexit(void (*func)(void));
/* 若成功返回0；若失败返回非0 */
```

`exit`将自动调用`atexit`登记的终止处理程序，调用的顺序与登记的顺序相反，若登记多次也会调用多次

```c
#include <stdlib.h>
#include <stdio.h>

static void func1()
{
    puts("func1");
}

static void func2()
{
    puts("func2");
}

int main()
{
    if (atexit(func1) != 0)
        exit(1);
    if (atexit(func2) != 0)
        exit(1);
    if (atexit(func2) != 0) /* 登记两次 */
        exit(1);

    puts("main");
    exit(0);
}
```

上面程序的输出为

```
main
func2
func2
func1
```

其中，func2函数调用了两次，把`main`函数最后的`exit(0)`改为`return 0`效果相同

### `int main(int argc, char *argv[]);`

对于C程序的`main`函数`int main(int argc, char *argv[])`, ISO C和POSIX都要求`argv[argc]`是一个空指针

```c
int main(int argc, char **argv) {
    printf("%p\n", argv[argc]); // (nil)
}
```

### `environ`

全局变量`extern char **environ;` 包含环境变量表的地址，通常使用`environ`指针查看整个环境，用函数`getenv`, `putenv`访问特定的环境变量

```c
#include <stdio.h>
extern char **environ;
int main() {
    int i = 0;
    while (environ[i] != NULL) {
        printf("%s\n", environ[i]);
        i++;
    }
}
```

### C程序的存储空间布局

从低地址到高地址：正文段 --- 初始化数据段 --- 未初始化的数据(bss) --- 堆 --- 栈 --- 命令行参数与环境变量

### 存储空间分配

```c
#include <stdlib.h>
void *malloc(size_t size); /* 初始值不确定 */
void *calloc(size_t nobj, size_t size); /* 每一位都初始化为0 */
void *realloc(void *ptr, size_t newsize); /* 新增区域初始值不确定 */
/* 3个函数返回值：若成功返回分配的内存地址，否则返回空指针 */
void free(void *ptr);
```

`realloc`可能会移动存储区的位置，应当避免让指针指向该区域

对于`realloc`函数，若参数`ptr`为NULL，等价于`malloc`

### 环境变量

```c
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str);
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
```

`getenv`: 获取环境变量，若未找到，返回NULL

`putenv`: 取形式为name=value的字符串，放到环境表中，若name已存在则先删除原来的定义，成功返回0，否则返回非0

`setenv`: 设置name=value环境变量，若name已经存在且rewrite==0, 不覆盖；若name已经存在且rewrite!=0 ,进行覆盖，成功返回0，否则返回-1

`unsetenv`: 删除name环境变量，即使name不存在也不出错，成功返回0否则返回-1

```c
#include <stdlib.h>
#include <stdio.h>

void print_env(const char *name) {
    printf("%s\n", getenv(name) ?: "not found");
}

int main() {
    print_env("HOME");
    print_env("home");

    putenv("ABC=123");
    print_env("ABC");

    setenv("ABC", "456", 1);
    print_env("ABC");
    setenv("ABC", "789", 0);
    print_env("ABC");

    unsetenv("ABC");
    print_env("ABC");

    return 0;
}
```

输出为

```
/home/liph
not found
123
456
456
not found
```

这些设置环境变量的函数只影响当前进程及子进程，不影响父进程

`putenv` 与 `setenv`的区别：`setenv`必须分配存储空间，而`putenv`不必，因此，若字符串参数存放栈中并传递给`putenv`就会发生错误

### 进程的资源限制

```c
#include <sys/resource.h>

struct rlimit {
    rlim_t rlim_cur;    /* soft limit: current limit */
    rlim_t rlim_max;    /* hard limit: maximum value for rlim_cur */
}

int getrlimit(int resource, struct rlimit *rlptr);
int setrlimit(int resource, const struct rlimit *rlptr);
/* 若成功，两个函数返回0；否则返回非0 */
```

`getrlimit`, `setrlimit`在Single UNIX Specification的XSI扩展中定义，资源限制影响调用进程及子进程

在更改资源限制时，需遵循以下规则

1. 任何进程都可将软限制值更改为小于或等于其硬限制值
2. 任何进程都可降低其硬限制值，但必须大于等于软限制值
3. 超级用户进程才可以提高硬限制值

## 进程控制

### pid

```c
#include <unistd.h>

pid_t getpid(void);
// 返回进程id，不会失败。init进程id为1

pid_t getppid(void);
// 返回父进程id，不会失败。如果父进程退出，父进程将变为init(pid = 1)。
```

### 函数`fork, wait`

```c
#include <unistd.h>
pid_t fork(void);
```

子进程返回0，父进程返回子进程pid，出错则返回-1

```c
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

### 函数`exec`

```c
#include <unistd.h>
int execl(const char *pathname, const char *arg0, ... /* (char *)0 */ );
int execv(const char *pathname, char *const argv[]);
int execle(const char *pathname, const char *arg0, .../*(char *)0, char *const envp[]*/);
int execve(const char *pathname, char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg0, ... /* (char *)0 */ );
int execvp(const char *filename, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
```

### 函数`system`

```c
#include <stdlib.h>
int system(const char *cmdstring);
```

## /proc 文件系统

```shell
$ cat /proc/1/status
Name:   systemd
Umask:  0000
State:  S (sleeping)
Tgid:   1
Ngid:   0
Pid:    1
PPid:   0
TracerPid:      0
Uid:    0       0       0       0
Gid:    0       0       0       0
FDSize: 128
Groups:
NStgid: 1
NSpid:  1
NSpgid: 1
NSsid:  1
VmPeak:   232968 kB
VmSize:   167820 kB
VmLck:         0 kB
VmPin:         0 kB
VmHWM:     11864 kB
VmRSS:     11864 kB
RssAnon:            2880 kB
RssFile:            8984 kB
RssShmem:              0 kB
VmData:    18760 kB
VmStk:       132 kB
VmExe:        40 kB
VmLib:     10744 kB
VmPTE:        88 kB
VmSwap:        0 kB
HugetlbPages:          0 kB
CoreDumping:    0
THP_enabled:    1
Threads:        1
SigQ:   1/15563
SigPnd: 0000000000000000
ShdPnd: 0000000000000000
SigBlk: 7fe3c0fe28014a03
SigIgn: 0000000000001000
SigCgt: 00000001800004ec
CapInh: 0000000000000000
CapPrm: 000001ffffffffff
CapEff: 000001ffffffffff
CapBnd: 000001ffffffffff
CapAmb: 0000000000000000
NoNewPrivs:     0
Seccomp:        0
Seccomp_filters:        0
Speculation_Store_Bypass:       vulnerable
SpeculationIndirectBranch:      always enabled
Cpus_allowed:   f
Cpus_allowed_list:      0-3
Mems_allowed:   00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:      0
voluntary_ctxt_switches:        4836
nonvoluntary_ctxt_switches:     344
$ cat /proc/1/cmdline
/sbin/init
$ sudo readlink /proc/1/cwd
/
$ sudo cat /proc/1/environ | tr '\0' '\n'
SHLVL=1
HOME=/
init=/sbin/init
TERM=linux
BOOT_IMAGE=/boot/vmlinuz-5.18.0-2-amd64
drop_caps=
PATH=/sbin:/usr/sbin:/bin:/usr/bin
PWD=/
rootmnt=/root
$ sudo readlink /proc/1/exe
/usr/lib/systemd/systemd
$ sudo ls /proc/1/fd
...
$ sudo cat /proc/1/maps
...
```
