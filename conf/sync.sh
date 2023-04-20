#!/bin/bash

set -xe

for dir in `ls -d */`; do
    cd $dir
    if [ -f sync.sh ]; then
        bash sync.sh
    fi
    cd -
done
