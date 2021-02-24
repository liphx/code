# basename, dirname

```
basename NAME [SUFFIX]
basename OPTION... NAME...
去除路径目录前缀（以及后缀）

dirname [OPTION] NAME...
去除文件名的最后一个组成部分
```

```shell
$ basename tmp/0.c.cpp
0.c.cpp
$ basename tmp/0.c.cpp .cpp
0.c
$ basename tmp/0.c.cpp .c.cpp
0
$ dirname /
/
$ dirname /home/
/
$ dirname /bin/bash
/bin
```
