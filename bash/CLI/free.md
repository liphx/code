# free  - 显示系统中已用和未用的内存空间总和

```shell
$ free -w -h
              total        used        free      shared     buffers       cache   available
Mem:           31Gi       2.6Gi        27Gi       207Mi        69Mi       1.5Gi        28Gi
Swap:            0B          0B          0B
```

输出情况

```
total       物理内存和交换空间总大小
used        已经被使用的
free        剩余可用的
shared      共享使用的物理内存
buffers     buffer使用的物理内存
cache       cache使用的物理内存
available   还可以被应用程序使用的物理内存
```

其他参数

```
 -b, --bytes         show output in bytes
     --kilo          show output in kilobytes
     --mega          show output in megabytes
     --giga          show output in gigabytes
     --tera          show output in terabytes
     --peta          show output in petabytes
 -k, --kibi          show output in kibibytes
 -m, --mebi          show output in mebibytes
 -g, --gibi          show output in gibibytes
     --tebi          show output in tebibytes
     --pebi          show output in pebibytes
 -h, --human         show human-readable output
     --si            use powers of 1000 not 1024
 -l, --lohi          show detailed low and high memory statistics
 -t, --total         show total for RAM + swap
 -s N, --seconds N   repeat printing every N seconds
 -c N, --count N     repeat printing N times, then exit
 -w, --wide          wide output
```

另外，查看文件 `/proc/meminfo` 同样可以获得上面这些信息

```shell
$ cat /proc/meminfo
```
