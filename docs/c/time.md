# 时间

## C标准库函数

```c
#include <time.h>
struct tm {
    int tm_sec;        /* Seconds.     [0-60] (1 leap second) */
    int tm_min;        /* Minutes.     [0-59] */
    int tm_hour;       /* Hours.       [0-23] */
    int tm_mday;       /* Day.         [1-31] */
    int tm_mon;        /* Month.       [0-11] */
    int tm_year;       /* Year - 1900.  */
    int tm_wday;       /* Day of week. [0-6] */
    int tm_yday;       /* Days in year.[0-365] */
    int tm_isdst;      /* DST.         [-1/0/1]*/
};
typedef /* unspecified */ time_t;
// 尽管 C 标准没有定义，它几乎总是一个保有从 UTC 1970 年 1 月 1 日 00:00 开始秒数的整数值（不计闰秒）
typedef /* unspecified */ clock_t;
// 足以表示进程所用的处理器时间的类型
struct timespec {
    time_t tv_sec;  /* seconds */
    long tv_nsec;   /* nanoseconds */
};
clock_t clock(void);
time_t time(time_t *timer);
double difftime(time_t time1, time_t time0);
// return time1 - time0
time_t mktime(struct tm *timeptr);
// converts the broken-down time, return time_t, -1 if error
```

## 函数 `gettimeofday`

```c
#include <sys/time.h>
int gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);
// 成功返回0，否则-1
struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
};
```

## 时区

```shell
$ ll /etc/localtime
lrwxrwxrwx 1 root root 33  6月 29 17:24 /etc/localtime -> /usr/share/zoneinfo/Asia/Shanghai
```

