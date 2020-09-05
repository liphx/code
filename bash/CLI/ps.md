# ps - process status

usage: `ps [options]`

1. ps 无任何选项只会显示当前控制台当前用户的进程

```bash
$ ps
  PID TTY          TIME CMD
13218 pts/1    00:00:00 bash
17444 pts/1    00:00:00 ps
```

2. ps -A, -e 显示全部进程
3. ps -f 完整格式输出

```bash
$ ps -f
UID        PID  PPID  C STIME TTY          TIME CMD
liph      6569  6536  0 22:05 pts/0    00:00:00 /bin/bash
liph     18311  6569  0 23:47 pts/0    00:00:00 ps -f
```

```
其中：
UID     启动进程的用户
PID     进程ID
PPID    父进程ID
C       CPU利用率
STIME   启动时的系统时间
TTY     终端
TIME    运行进程需要的累计CPU时间
CMD     程序
```
