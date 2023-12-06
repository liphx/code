**quick start with bazel**

```
project/
|-- WORKSPACE.bazel
|-- BUILD.bazel
`-- main.cpp
```

`WORKSPACE.bazel`

```
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "liph",
    remote = "https://github.com/liphx/code.git",
    branch = "main",
    strip_prefix = "src/cplusplus",
)
```

`BUILD.bazel`

```
cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        "@liph//liph",
    ],
)
```

`main.cpp`

```cpp
#include "liph/liph.h"

int main() {
    liph::print(std::vector<int>{10, 9, 42});
}
```

**env**

Linux & Mac

- [GCC 13.2.0](https://gcc.gnu.org/gcc-13/)
- [bazel 6.4.0](https://github.com/bazelbuild/bazel/releases)
