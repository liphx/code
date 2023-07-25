# liph's C++ codes

## quick start with bazel

```
project/
├── WORKSPACE.bazel
├── BUILD.bazel
└── main.cpp
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

## tools

```
Apple clang version 14.0.0 (clang-1400.0.29.202)
bazel 6.2.1-homebrew
```
