#!/bin/bash

set -xe

function sync_dir() {
    pushd "$1"
    if [ -f sync.sh ]; then
        bash sync.sh
    fi
    popd
}

if [ -z "$1" ] || [ "$1" = "all" ]; then
    for dir in $(ls -d ./*/); do
        sync_dir "$dir"
    done
else
    for dir in "$@"; do
        sync_dir "$dir"
    done
fi
