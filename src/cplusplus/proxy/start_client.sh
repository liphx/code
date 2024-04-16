#!/bin/bash

ulimit -c unlimited

while true; do
    ../bazel-bin/proxy/proxy --port 9981 --next_proxy 43.153.130.153 --next_proxy_port 9981 --username 1111 --password eeee
    sleep 1
done
