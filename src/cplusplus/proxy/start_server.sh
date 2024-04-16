#!/bin/bash

ulimit -c unlimited

while true; do
    ../bazel-bin/proxy/proxy --daemon --port 9981 --username 1111 --password eeee
    sleep 1
done
