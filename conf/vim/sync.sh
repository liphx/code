#!/bin/bash

set -xe

if [ ! -d ~/.vim/pack/bundle/start ]; then
    mkdir -p ~/.vim/pack/bundle/start
fi

if [ ! -d ~/.vim/pack/bundle/opt ]; then
    mkdir -p ~/.vim/pack/bundle/opt
fi

cp vimrc plugin.vim ~/.vim

# neovim

if [ ! -d ~/.config/nvim ]; then
    mkdir -p ~/.config/nvim
fi

cp init.lua ~/.config/nvim
# cp init.vim ~/.config/nvim
