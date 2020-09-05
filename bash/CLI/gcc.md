# gcc

以下面的`hello.c`源文件说明gcc的常见参数的用法

```c
#include <stdio.h>
#define STRING "hello, world"

int main()
{
    printf("%s\n", STRING);
    return 0;
}
```

```shell
$ gcc hello.c           # 1 - gcc file1 file2 ...， 完整的编译过程，生成 a.out
$ ./a.out
hello, world
$ gcc -x c++ hello.c    # 2 - 指定后面输入文件的语言，而非根据文件后缀判断
$ gcc -c hello.c        # 3 - 编译汇编但不连接，默认生成 hello.o
$ gcc -S hello.c        # 4 - 编译但不汇编，默认生成 hello.s
$ gcc -E hello.c -o hello.i     # 5 - 仅预处理，默认输出到标准输出，-o 指定输出文件
$ gcc hello.c -ansi     # 6 - 指定C语言标准，等价于 -std=c90 / -std=iso9899:1990
$ gcc hello.c -W -Wall  # 7 - 开启一些警告信息
$ gcc hello.c -g        # 8 - 为 gdb 产生额外的调试信息
$ gcc hello.c -O1       # 9 - 进行优化，还有-O2, -O3，不优化为-O0
```

其他编译选项

```
-Werror     视警告为错误，出现任何警告即放弃编译. 
-Os         相当于-O2.5
```
