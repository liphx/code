#!/bin/bash

dir=""

case `uname` in
    Darwin) dir="$HOME/Library/Application Support/Code/User/" ;;
    Linux) dir="$HOME/.config/Code/User/" ;;
    *) dir="$HOME/AppData/Roaming/Code/User/" ;;
esac

mkdir -p "$dir"
cp -f settings.json "$dir"
