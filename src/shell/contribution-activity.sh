#!/bin/bash

set -ex

function loop() {
    git commit --allow-empty -m 'BAZINGA'
    DATE=$(date +%s)
    for i in $(seq 1 $1); do
        ((DATE=$DATE-24*60*60))
        git commit --allow-empty --amend --date=$DATE -m 'BAZINGA'
        git push $2 $3 -f
    done
}

loop 365 origin master
