#!/bin/bash

set -xe

case `uname` in
    Darwin) cp -f ./mac.yml ~/.config/alacritty/alacritty.yml;;
    Linux) echo "No conf for Linux";;
    *) echo "No conf";;
esac
