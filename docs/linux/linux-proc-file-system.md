# Linux /proc 文件系统

`/proc` 目录包含的文件并未实际存储在磁盘上，而是内核在进程访问此类信息时动态创建而成

## /proc/PID

保存了进程相关的信息

```
cmdline     以\0 分隔的命令行参数
cwd         指向当前工作目录的符号链接
Environ     NAME=value 键值对环境列表，以\0 分隔
exe         指向正在执行文件的符号链接
fd          目录，包含了进程打开的文件描述符的符号链接
maps        内存映射
mem         进程虚拟内存
mounts      进程的安装点
root        指向根目录的符号链接（chroot 系统调用）
status      各种信息（进程ID、凭证、内存使用量、信号等）
task        线程目录
```

进程可通过 `/proc/self` 访问自身的 `/proc/PID`

```shell
$ cat /proc/self/cmdline
cat/proc/self/cmdline # \0结尾，无换行
$ readlink /proc/self/exe
/usr/bin/readlink
$ cd /tmp; readlink /proc/self/cwd
/tmp
```
