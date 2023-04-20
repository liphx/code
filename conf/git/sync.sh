#!/bin/bash

set -xe

config_dir=~/.config/git
if [ ! -d $config_dir ]; then
    mkdir -p $config_dir
fi

cp gitconfig $config_dir/config
cp gitmessage $config_dir/gitmessage
cp gitignore $config_dir/gitignore
