#!/bin/bash

set -xe

cd $(dirname $0)

if [ ! -d ~/.config/alacritty ]; then
    mkdir -p ~/.config/alacritty
fi
cp -f common.toml ~/.config/alacritty

if [ ! -d ~/.config/alacritty/base16-alacritty ]; then
    git clone https://github.com/aarowill/base16-alacritty.git ~/.config/alacritty/base16-alacritty
fi

cp -f mac.toml ~/.config/alacritty/alacritty.toml
