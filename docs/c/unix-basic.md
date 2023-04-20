# UNIX基础

## UNIX基础知识

1、文件名：只有斜线和空字符不能出现在文件名中，推荐将文件名限制在`a-z`, `A-Z`, `0-9`, `.`, `-`, `_`

2、函数`exit`终止程序，按惯例，参数0表示正常结束，1-255表示出错

3、`errno` 是一个预处理器宏，展开成可修改的整形左值，`errno` 的值在程序启动时为0，允许库函数将正整数写入 `errno` ，任何函数都不会将`errno`设为0

```c
#include <string.h>
char *strerror(int errnum);

#include <stdio.h>
void perror(const char *msg); /* 在标准错误上输出msg，冒号，空格，相应error的出错信息，换行符 */
```
4、出错恢复：致命错误，无法执行恢复动作；非致命错误，有时可以妥善处理
5、附属组id：`/etc/passwd`中记录着用户id与对应的组id，`/etc/group`中可以查看用户所在的附属组id

6、UNIX时间，`time_t`记录日历时间，自1970年1月1日00:00:00以来经过的秒数；`clock_t`度量进程使用CPU资源，以时钟滴答计算
UNIX为进程维护了3个进程时间值，时钟时间、用户CPU时间、系统CPU时间

7、系统调用（`system call`）和库函数：进入内核的入口点被称为系统调用，每个系统调用在标准C库中设置有一个同名的函数，以`read`系统调用为例，用户程序在用户空间将参数压入栈中，调用**read函数**，进行**库过程read**，陷入内核，进行**read系统调用**

## UNIX标准及实现

1、ISO C：提供C程序的可移植性，使其适合于大量不同的操作系统

2、IEEE POSIX(Portable Operating System Interface)：可移植操作系统接口

3、Single UNIX Specification：单一UNIX规范，POSIX.1标准的一个超集

4、UNIX系统实现：SVR4(System V Release 4)，4.4BSD(Berkeley Software Distribution)，FreeBSD，Linux，Mac OS X，Solaris

5、限制：编译时限制（如短整型最大值，可在头文件中定义）；运行时限制（如文件名有多少个字符，要求进程调用一个函数获得限制值）

6、函数`sysconf`, `pathconf`, `fpathconf` ：运行时限制

```c
#include <unistd.h>
long sysconf(int name);
long pathconf(const char *pathname, int name);
long fpathconf(int fd, int name);
/* 三个函数若出错，均返回-1 */
```
