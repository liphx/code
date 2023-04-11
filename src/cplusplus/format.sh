#!/bin/bash

set -e

cd $(dirname $(readlink -f $0))/../..  # src/cplusplus
for f in $(git diff --cached --name-only --diff-filter=AM); do
    if [[ $f =~ .*\.(cpp|h|hpp|cc)$ ]]; then
        echo "clang-format -i $f"
        clang-format -i "$f"
    fi
done
