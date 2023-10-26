#!/bin/bash

set -e

# cd ~/usr/bin/
# wget https://github.com/bazelbuild/bazel/releases/download/6.3.2/bazel-6.3.2-linux-x86_64
# chmod +x bazel-6.3.2-linux-x86_64
# ln -s bazel-6.3.2-linux-x86_64 bazel

# make liph/liph.h
echo "#ifndef LIPH_LIPH_H_" > liph/liph.h
echo "#define LIPH_LIPH_H_" >> liph/liph.h
echo >> liph/liph.h
echo '// clang-format off' >> liph/liph.h
for i in `find liph -type f -name '*.h' | grep -v liph.h | sort`; do echo "#include \"$i\""; done >> liph/liph.h
echo '// clang-format on' >> liph/liph.h
echo >> liph/liph.h
echo "#endif  // LIPH_LIPH_H_" >> liph/liph.h

# clang-format
cplusplus_root=$(dirname $(readlink -f $0))
git_root=$cplusplus_root/../..
cd $git_root
for f in $(git diff --cached --name-only --diff-filter=AM); do
    if [[ $f =~ .*\.(cpp|h|hpp|cc)$ ]]; then
        echo "clang-format -i $f"
        clang-format -i "$f"
    fi
done
cd $cplusplus_root

# build
bazel build ...

# test
./bazel-bin/liph/test
# ./bazel-bin/liph/test --gtest_also_run_disabled_tests

# check headers
./bazel-bin/tools/check-cpp-header liph

# doxygen
cd $cplusplus_root/liph
doxygen

# ltl
cd $cplusplus_root/ltl
GTEST_PATH=../bazel-bin/external/thirdparty/googletest-1.14.0
make
export LD_LIBRARY_PATH=$GTEST_PATH:$LD_LIBRARY_PATH
export DYLD_FALLBACK_LIBRARY_PATH=$GTEST_PATH:$DYLD_FALLBACK_LIBRARY_PATH
./test_all && ./test_std_all

# cmake
# cmake -S .- B build
# cmake --build build
# ./build/main
