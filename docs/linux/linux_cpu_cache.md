Linux cpu cache命中率

```shell
$ sudo apt install linux-perf

$ cat /proc/sys/kernel/perf_event_paranoid
-1

$ g++ -g -std=c++11 main.cpp && perf record -a ./a.out && perf report
$ perf stat ./a.out

 Performance counter stats for './a.out':

            786.25 msec task-clock                #    0.998 CPUs utilized          # cpu利用率
                 7      context-switches          #    0.009 K/sec                  # 进程切换次数
                 0      cpu-migrations            #    0.000 K/sec                  # CPU迁移，使用另一个核调度
                42      page-faults               #    0.053 K/sec
   <not supported>      cycles                                                                                                   # 处理器时钟
   <not supported>      instructions                                                                                               # 机器指令数目
   <not supported>      branches
   <not supported>      branch-misses

       0.787566384 seconds time elapsed

       0.786847000 seconds user
       0.000000000 seconds sys

$ perf stat -e cache-misses,cache-references -r 5 ./a.out

 Performance counter stats for './a.out' (5 runs):

            14,539      cache-misses                                                  ( +-  9.11% )

       0.165718286 seconds time elapsed                                          ( +-  0.73% )

$ LD_LIBRARY_PATH=/docker/opt/rh/devtoolset-3/root/usr/lib64 perf top -p $(pidof sf) -e cache-misses,cache-references

1min
压缩 qpm(2.12K)
Available samples
616K cache-misses
661K cache-references

未压缩 qpm(2.33K)
Available samples
741K cache-misses
787K cache-references

>>> 616/661
0.9319213313161876
>>> 741/787
0.9415501905972046

```

```cpp
#include <cstdint>

const int N = 1024;
int64_t arr[N][N][N];

int main() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                arr[i][j][k]++;
}
```

```shell
$ diff main.cpp main2.cpp  -u
--- main.cpp    2022-06-29 15:50:20.015520972 +0800
+++ main2.cpp   2022-06-29 15:45:00.561930419 +0800
@@ -7,5 +7,5 @@
     for (int i = 0; i < N; i++)
         for (int j = 0; j < N; j++)
             for (int k = 0; k < N; k++)
-                arr[i][j][k]++;
+                arr[k][j][i]++;
 }
```

```shell
$ perf stat -e cache-misses,cache-references ./test1

 Performance counter stats for './test1':

        17,353,890      cache-misses              #   17.681 % of all cache refs
        98,152,559      cache-references

       5.383523208 seconds time elapsed

dev:~$ perf stat -e cache-misses,cache-references ./test2

 Performance counter stats for './test2':

     1,118,657,362      cache-misses              #   73.016 % of all cache refs
     1,532,064,546      cache-references

      32.273535127 seconds time elapsed

```

cache memories

```shell
$ getconf -a | grep CACHE
LEVEL1_ICACHE_SIZE                 32768
LEVEL1_ICACHE_ASSOC                8
LEVEL1_ICACHE_LINESIZE             64
LEVEL1_DCACHE_SIZE                 32768
LEVEL1_DCACHE_ASSOC                8
LEVEL1_DCACHE_LINESIZE             64
LEVEL2_CACHE_SIZE                  524288
LEVEL2_CACHE_ASSOC                 8
LEVEL2_CACHE_LINESIZE              64
LEVEL3_CACHE_SIZE                  268435456
LEVEL3_CACHE_ASSOC                 0
LEVEL3_CACHE_LINESIZE              64
LEVEL4_CACHE_SIZE                  0
LEVEL4_CACHE_ASSOC                 0
LEVEL4_CACHE_LINESIZE              0
$ cat /proc/cpuinfo
cache size      : 512 KB
cache_alignment : 64
$ lscpu
L1d cache:             32K
L1i cache:             32K
L2 cache:              512K
L3 cache:              32768K

```

## 局部性（locality）

时间局部性：引用过的内存位置，不久后再次引用。

空间局部性：引用过的内存位置，不久后引用附件的位置。

硬件上：高速缓存存储器保存最近被引用的指令和数据项。

操作系统：使用主存作为虚拟地址空间最近被引用块的高速缓存。

```shell
$ free
             total       used       free     shared    buffers     cached
Mem:      67108864   44951396   22157468          0          0   42561568
-/+ buffers/cache:    2389828   64719036
Swap:      2097152      59532    2037620

# total = used + free
```

Page cache



```
perf top -p $(pidof sf) -e cache-references,cache-misses,cycles,instructions,branches,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses
```

```
```

perf record -g -p $(pidof sf)

