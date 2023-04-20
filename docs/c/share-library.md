# 共享库

## 静态库

C语言本身没有模块或包的概念，C代码构建程序主要通过操作系统的库来实现模块复用

以三个源文件为例 `mod1.c` `mod2.c`  `mod3.c`

```shell
gcc -c mod1.c mod2.c mod3.c         # 将这些文件分别编译成目标文件
ar r libmod.a mod1.o mod2.o mod3.o  # 创建静态库
ar tv libmod.a                      # 显示归档中的目录表
ar d libmod.a mod3.o                # 删除一个模块
ar r libmod.a mod3.o                # ar r 可以用来添加或更新一个模块
gcc main.c -L. -lmod                # 链接静态库
gcc main.c libmod.a                 # 或通过指定静态库的名字来链接
```

静态库的查找发生在链接时，通过 `-L` 指定额外的搜索路径，`gcc -print-search-dirs` 显示gcc 默认的标准搜索路径

## 共享库

```shell
gcc -c -fPIC mod1.c mod2.c mod3.c   # 生成位置独立的代码
gcc -shared -o libmod.so mod1.o mod2.o mod3.o       # 创建共享库

gcc -fPIC mod1.c mod2.c mod3.c -shared -o libmod.so # 或使用一条命令创建
```

可通过以下命令检查目标文件在编译时是否启用了 `-fPIC`

```shell
nm mod1.o | grep _GLOBAL_OFFSET_TABLE_
readelf -s mod1.o | grep _GLOBAL_OFFSET_TABLE_
```

如果以下命令有相应的输出表示共享库中至少有一个目标模块编译时没有指定 `-fPIC`

```shell
objdump --all-headers libmod.so | grep TEXTREL
readelf -d libmod.so | grep TEXTREL
```

使用共享库比静态库多做的两件事

- 找到运行时所需的共享库(在链接阶段将共享库的名字嵌入可执行文件中)
- 运行时解析嵌入的库名，如果库不在内存中就将库加载进内存

### dlopen, dlsym

`hello.c`

```c
#include <stdio.h>

void hello()
{
    puts("hello called");
}
```

`main.c`

```c
#include <stdio.h>
#include <dlfcn.h>

typedef void (*pf)();

int main()
{
    void *handle = dlopen("./hello.so", RTLD_NOW | RTLD_GLOBAL);
    pf start = dlsym(handle, "hello");
    start();
}
```

```shell
$ gcc -fPIC -shared hello.c -o hello.so
$ gcc main.c -ldl
$ ./a.out
hello called
```
