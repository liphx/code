#!/bin/bash

set -xe

cd $(dirname $0)

cp -f user-dirs.dirs $HOME/.config/user-dirs.dirs

# cd $HOME
# mkdir Desktop Download Templates Public Documents Music Pictures Videos
