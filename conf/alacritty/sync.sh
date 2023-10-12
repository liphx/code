#!/bin/bash

set -xe

if [ ! -d ~/.config/alacritty ]; then
    mkdir -p ~/.config/alacritty
fi

if [ ! -d ~/.config/alacritty/base16-alacritty ]; then
    git clone https://github.com/aarowill/base16-alacritty.git ~/.config/alacritty/base16-alacritty
fi

case `uname` in
    Darwin) cp -f ./mac.yml ~/.config/alacritty/alacritty.yml;;
    Linux) cp -f ./linux.yml ~/.config/alacritty/alacritty.yml;;
    *MINGW*) cp -f windows.yml ~/AppData/Roaming/alacritty/alacritty.yml;;
    *) echo "No conf";;
esac
