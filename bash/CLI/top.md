# top - 实时显示进程信息

```shell
$ top

top - 17:14:49 up 38 min,  1 user,  load average: 0.34, 0.32, 0.27
Tasks: 231 total,   1 running, 230 sleeping,   0 stopped,   0 zombie
%Cpu(s):  1.3 us,  0.6 sy,  0.0 ni, 98.0 id,  0.0 wa,  0.0 hi,  0.2 si,  0.0 st
MiB Mem :  32106.1 total,  28827.0 free,   2035.4 used,   1243.7 buff/cache
MiB Swap:      0.0 total,      0.0 free,      0.0 used.  29543.0 avail Mem 

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND                                               
 1486 liph      20   0 2218084 175772 102740 S   2.6   0.5   0:49.15 deepin-music                                          
  911 root      20   0  372116 108980  73316 S   2.3   0.3   0:36.82 Xorg                                                  
 1515 liph      20   0  678280  95100  49044 S   2.3   0.3   0:14.91 deepin-terminal                                       
 1093 liph      20   0 3668852 413004 130560 S   2.0   1.3   1:47.33 gnome-shell
...
```

``` 
其中：
PID     进程ID
USER    进程属主
PR      进程优先级
NI      进程谦让度值
VIRT    进程占用的虚拟内存总量
RES     进程占用的物理内存总量
SHR     进程和其他进程共享的内存总量
S       进程的状态，其中，D代表可中断的休眠状态，R代表在运行状态，S代表休眠状态，T代表跟踪状态或停止状态，Z代表僵化状态
%CPU    进程使用的CPU时间比例
%MEM    进程使用的内存占可用内存的比例
TIME+   自进程启动到目前为止的CPU时间总量
COMMAND 程序名
```