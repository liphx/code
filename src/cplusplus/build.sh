#!/bin/bash

set -e

# make liph/liph.h
echo "#ifndef LIPH_LIPH_H_" > liph/liph.h
echo "#define LIPH_LIPH_H_" >> liph/liph.h
echo >> liph/liph.h
for i in `find liph -type f -name '*.h' | grep -v liph.h | sort`; do echo "#include \"$i\""; done >> liph/liph.h
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

# ltl
cd $cplusplus_root/ltl
GTEST_PATH=../bazel-bin/external/thirdparty/googletest-1.14.0
make
export LD_LIBRARY_PATH=$GTEST_PATH
export DYLD_FALLBACK_LIBRARY_PATH=$GTEST_PATH
./test_all && ./test_std_all
