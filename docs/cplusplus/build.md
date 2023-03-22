# c/cpp build tools


## CMake

一些链接

* <https://cmake.org/>
* <https://github.com/Kitware/CMake/>

一些书籍

<http://cliutils.gitlab.io/modern-cmake/>

### 安装（升级）cmake

Debian stable 10.7, apt 源里的cmake 版本是3.13.4, 最新CMake(2021-2-2) 已经发布了 3.19.4版本。由于我的系统已经不满足不少项目里指定的`cmake_minimum_required`， 因此我需要更新一下Cmake。

最新的cmake 可以在 <https://cmake.org/download/> 获取，实际上它的下载链接指向 github Releases 页面。下载页面有二进制和源代码版本，我打算从源代码安装。

```shell
$ wget https://github.com/Kitware/CMake/releases/download/v3.19.4/cmake-3.19.4.tar.gz
$ sudo tar xvf cmake-3.19.4.tar.gz -C /opt
$ cd /opt/cmake-3.19.4/
$ sudo ./configure
$ sudo make
$ echo 'export PATH=/opt/cmake-3.19.4/bin:$PATH' >> ~/.profile
$ source ~/.profile
$ cmake --version
cmake version 3.19.4

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

3.19 版本的参考文档可以在 <https://cmake.org/cmake/help/v3.19/> 获取。

### 使用

正如使用tarball 编译软件的经典步骤`./configure && make && make install` 一样，使用cmake 构建项目也有经典的步骤

```shell
mkdir build
cd build
cmake ..
make
```

或者使用

```shell
cmake -S . -B build
cmake --build build
```

这些选项都可以通过`cmake --help` 获取，使用后者的好处是不用频繁切换目录

### 简单的例子

hello 目录下两个文件，`hello.c` 以及 `CMakeLists.txt`

```c
#include <stdio.h>

int main()
{
    printf("hello, world\n");
    return 0;
}
```

```cmake
cmake_minimum_required(VERSION 3.13)

project(hello)

add_executable(hello hello.c)
```

执行 `cmake -S . -B build` 指定源代码目录与构建目录，终端会输出编译器与系统信息

再执行 `cmake --build build` 则会进行编译步骤，同时会输出编译信息

现在来看文件`CMakeLists.txt`

* `cmake_minimum_required` 指定了构建需要的cmake 的最低版本
* `project` 指定项目名称
* `add_executable` 生成可执行文件

再来看一个构建库的例子

文件 `add.c` 以及 `CMakeLists.txt`

```c
int add(int x, int y)
{
    return x + y;
}
```

```cmake
project(math)

add_library(math SHARED add.c)
```

构建完成会生成`libmath.so`，`add_library` 不加`SHARED`或指定`STATIC` 则会构建静态库 `libmath.a`

`CMakeLists.txt` 里的指令是大小写不敏感的

### 另一个例子

```
.
|-- CMakeLists.txt
|-- hello.cpp
|-- hello.h.in
`-- mylib
    |-- add.cpp
    |-- add.h
    `-- CMakeLists.txt
```

顶层 `CMakeLists.txt`

```cmake
# cmake minimum version
cmake_minimum_required(VERSION 3.13)

# set the project name and version
project(hello   VERSION 1.0
                DESCRIPTION "A simple project")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(hello.h.in hello.h)

add_subdirectory(mylib)

add_executable(hello hello.cpp)

target_link_libraries(hello PUBLIC mylib)

target_include_directories(hello PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/mylib"
                        )
```

`hello.h.in`

```c
#ifndef HELLO_H_
#define HELLO_H_

#define HELLO_VERSION_MAJOR @hello_VERSION_MAJOR@
#define HELLO_VERSION_MINOR @hello_VERSION_MINOR@

#endif // HELLO_H_
```

`hello.cpp`

```cpp
#include "hello.h"
#include "add.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    printf("hello version: %d.%d\n", HELLO_VERSION_MAJOR, HELLO_VERSION_MINOR);
    printf("add(1, 2) = %d\n", add(1, 2));
}
```

`mylib/CMakeLists.txt`

```cmake
add_library(mylib add.cpp)
```

## Bazel

* <https://bazel.build>
* <https://github.com/bazelbuild/bazel>
* <https://github.com/bazelbuild/examples>

### Workspace, Repositories, Packages, Targets, Labels

```
project/
|-- package1
|   `-- BUILD
|-- package2
|   `-- BUILD
`-- WORKSPACE
```

`WORKSPACE` 文件标识了项目的根目录，这个目录下也会存放 bazel 的输出。`WORKSPACE` 可以为空，也可以包含对外部依赖的引用。代码以仓库（Repositories）的形式组织，包含 `WORKSPACE` 文件的目录称为主仓库（main repository, `@`）。

含有 `BUILD` 的目录是一个包。包包含该目录下的所有文件，包括子目录（含有`BUILD`文件的子目录除外，它是另一个包）。包中的元素称为目标（Targets），有以下几类目标。

* 文件（File）
  * 源文件（Source File）
  * 生成的文件（Generated File）
* 规则（Rule）
* 包组（Package Groups）

目标的名称称为标签（Label），例如 `@myrepo//my/app/main:app_binary`（`@myrepo` 内部可以简写为 `//my/app/main:app_binary`）

标签由冒号分为两部分，包名（`my/app/main`）和 目标名（`app_binary`）, 当省略冒号时，目标名等同包名最后一段路径，例如
`//my/app` 等价于 `//my/app:app`。引用当前包内目标时，包名也可以省略，因此以下四种写法等价。

```bazel
# /文件 /my/app:BUILD 内
//my/app:app
//my/app
:app
app
```

BUILD 文件中可以定义规则，用以指定输入输出间的关系及构建输出的步骤。

### Workspace Rules

```bazel
local_repository(name, path, repo_mapping)
new_local_repository(name, build_file, build_file_content, path, repo_mapping, workspace_file, workspace_file_content)
```

例如

```bazel
# 通过 @my-ssl 引用该仓库
local_repository(
    name = "my-ssl",
    path = "/home/user/ssl",
)

new_local_repository(
    name = "my-ssl",
    path = "/home/user/ssl",
    build_file = "BUILD.my-ssl",
)
```

`BUILD.my-ssl` 如下

```bazel
java_library(
    name = "openssl",
    srcs = glob(['*.java'])
    visibility = ["//visibility:public"],
)
```

可以不指定 `build_file` 而是指定 `build_file_content` （必须有其中一个），以包含文件中的内容（注意缩进要和BUILD文件一致）

```bazel
# Loading an extension
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(name, branch, commit, init_submodules, patch_args, patch_cmds, patch_cmds_win,
               patch_tool, patches, recursive_init_submodules, remote, shallow_since, strip_prefix,
               tag, verbose)

new_git_repository(name, branch, build_file, build_file_content, commit, init_submodules,
                   patch_args, patch_cmds, patch_cmds_win, patch_tool, patches,
                   recursive_init_submodules, remote, shallow_since, strip_prefix, tag, verbose,
                   workspace_file, workspace_file_content)

http_archive(name, auth_patterns, build_file, build_file_content, canonical_id, netrc, patch_args,
             patch_cmds, patch_cmds_win, patch_tool, patches, sha256, strip_prefix, type, url, urls,
             workspace_file, workspace_file_content)

http_file(name, auth_patterns, canonical_id, downloaded_file_path, executable, netrc, sha256, urls)

http_jar(name, auth_patterns, canonical_id, netrc, sha256, url, urls)
```

### BUILD Rules

#### C/C++ Rules

```bazel
cc_binary
cc_import
cc_library
cc_proto_library
fdo_prefetch_hints
fdo_profile
propeller_optimize
cc_test
cc_toolchain
cc_toolchain_suite
```

### Bazel CLI

```shell
bazel clean             # 删除输出文件
bazel build <target>    # 构建目标
```
