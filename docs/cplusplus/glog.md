# glog

https://github.com/google/glog

## bazel build

```bazel
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)
```

```bazel
cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = ["@com_github_google_glog//:glog"],
)
```

## quick start

Severity Levels: INFO, WARNING, ERROR, and FATAL

Logging a FATAL message terminates the program (after the message is logged)

Messages of a given severity are logged not only in the logfile for that severity, but also in all logfiles of lower severity.

默认日志文件

```
/tmp/\<program name\>.\<hostname\>.\<user name\>.log.\<severity level\>.\<date\>-\<time\>.\<pid\>
```

glog copies the log messages of severity level ERROR or FATAL to standard error (stderr) in addition to log files.

```cpp
#include "glog/logging.h"

int main(int, char **argv) {
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "info log";
    LOG(WARNING) << "warning log";
    LOG(ERROR) << "error log";
    LOG(FATAL) << "fatal log";
}
```

```shell
$ ./a.out
E20230131 16:13:24.209595 1260326 glog.cpp:7] error log
F20230131 16:13:24.209903 1260326 glog.cpp:8] fatal log
*** Check failure stack trace: ***
    @        0x1035aee0d  google::LogMessage::Fail()
    @        0x1035ad56f  google::LogMessage::SendToLog()
    @        0x1035ae3da  google::LogMessage::Flush()
    @        0x1035b3519  google::LogMessageFatal::~LogMessageFatal()
    @        0x1035af475  google::LogMessageFatal::~LogMessageFatal()
    @        0x1034d2255  main
    @     0x7ff81683d310  start
Abort trap: 6
```

## flags

```shell
./a.out --log_dir=log    # if gflags is installed
GLOG_log_dir=log ./a.out # else
```

- log_dir (string, default="")
- minloglevel (int, default=0, which is INFO)
- max_log_size(uint32, 1800)
