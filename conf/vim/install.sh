#!/bin/bash

set -ex

git clone https://github.com/vim/vim.git ~/usr/src/vim
cd ~/usr/src/vim
./configure --prefix="$HOME/usr" --enable-python3interp=yes
make -j $(nproc --all)
make install
