# gcc

## gcc 与 g++ 的区别

1. gcc 把 `.c` 文件当做 C 程序，把 `.cpp` 文件当做 C++ 程序，g++ 把两者都看成 C++ 程序，两者可以通过 `-x <language>` 指定语言
2. g++ 会自动链接 C++ 标准库，gcc 需要手动链接 (`-lstdc++`)

## gcc 选项

| 选项 | 描述 |
| -- | -- |
| `-x <lang>` | 指定语言，用于选项后面的所有文件 |
| `-E` | 预处理，在标准输出打印 |
| `-c` | 编译并汇编，生成 `.o` 文件 |
| `-S` | 编译不汇编，生成 `.s` 文件 |
| `-o <file>` | 指定输出文件 |
| `-std=c++11` | 指定 C++ 标准(98, 11, 14, 17, 20) |
| `-g` | 生成调试信息，供 gdb 使用 |
| `-O1` | 优化选项，`O1, O2, O3` 优化递增， `-O0` 不优化 |
| `-Dmacro` | 定义宏 macro，宏的内容定义为字符串 "1"  |
| `-Dmacro=def` | 定义宏 macro 的内容为 def |
| `-Umacro` | 取消宏 macro |
| `-Idir` | 指定头文件搜索路径 |
| `-Ldir` | 指定连接搜索路径 |
| `-lname` | 连接 libname.a libname.so |
| `-fpic` | 生成位置无关目标代码，适用于共享库 |
| `-fPIC` | 生成位置无关目标代码，适用于动态连接 |
| `-shared` | 生成一个共享目标文件 |
| `-w` | 禁止所有警告信息 |
| `-W` | 显示额外的警告信息 |
| `-Wall` | 显示更多的警告信息 |
| `-Wshadow` | 警告局部变量屏蔽了另一个局部变量的情况 |
| `-Werror` | 视警告为错误 |

## fpic fPIC 的区别

[stackoverflow](https://stackoverflow.com/questions/3544035/what-is-the-difference-between-fpic-and-fpic-gcc-parameters)

## ASAN

| gcc 选项                      | 描述              |
| ----------------------------- | ----------------- |
| `-fsanitize=address`          | 开启内存越界检测  |
| `-fsanitize-recover=address`  | 遇到错误不退出    |

`ASAN_OPTIONS=halt_on_error=0:log_path=asan.log`

## gcc/g++ 版本

## CPPFLAGS/CXXFLAGS

[stackoverflow](https://stackoverflow.com/questions/495598/difference-between-cppflags-and-cxxflags-in-gnu-make)

## Include What You Use

<https://github.com/include-what-you-use/include-what-you-use>

```shell
sudo apt install llvm libclang1 libclang-dev clang
git clone https://github.com/include-what-you-use/include-what-you-use.git
cd include-what-you-use/
git checkout clang_13
mkdir ../build && cd ../build
cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-13 ../include-what-you-use
make
./bin/include-what-you-use $CXXFLAGS file.cpp
```

gcc 4.9.2 文档

- https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/
- https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Debugging-Options.html

[gcc -fomit-frame-pointer](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

