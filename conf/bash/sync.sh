#!/bin/bash

set -xe

cd $(dirname $0)

cp -f ./profile ~/.bash_profile
cp -f ./bashrc ~/.bashrc
cp -f ./bash_logout ~/.bash_logout

if [ ! -d ~/.bash ]; then
    mkdir ~/.bash
fi

cp -f aliases.sh scripts.sh ssh.sh ~/.bash
touch ~/.bash/local.sh # specific machine
