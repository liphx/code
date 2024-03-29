#!/bin/bash

set -e

cplusplus_root=$(dirname $(readlink -f $0))
git_root=$cplusplus_root/../..
docs_root=$git_root/docs

# version=6.4.0
# platform=x86_64
# os=linux or darwin
# cd ~/usr/bin/
# wget https://github.com/bazelbuild/bazel/releases/download/$version/bazel-$version-$os-$platform
# chmod +x bazel-$version-$os-$platform
# ln -sf bazel-$version-$os-$platform bazel

# make liph/liph.h
echo "#ifndef LIPH_LIPH_H_" > liph/liph.h
echo "#define LIPH_LIPH_H_" >> liph/liph.h
echo >> liph/liph.h
echo '// clang-format off' >> liph/liph.h
for i in $(find liph -type f -name '*.h' | grep -v liph.h | sort); do echo "#include \"$i\""; done >> liph/liph.h
echo '// clang-format on' >> liph/liph.h
echo >> liph/liph.h
echo "#endif  // LIPH_LIPH_H_" >> liph/liph.h

# clang-format
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

# doxygen 1.9.8
# git submodule update --init
if [[ $1 = 'docs' ]]; then
    rm -rf $docs_root
    mkdir $docs_root
    doxygen
    echo 'cpp.lipenghua.com' > $docs_root/CNAME
fi

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
