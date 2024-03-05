#!/bin/bash

set -e

function update() {
    cd $1 && git pull
}

update ~/liphx.github.io
update ~/elden_ring

# crontab -l
# */5 * * * * ~/update-git-repo.sh > /dev/null 2>&1
