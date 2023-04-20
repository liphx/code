#!/bin/bash

set -e

cd /home/liph/static.liph.ink
git pull origin main
python3 build.py

# crontab -l
# */5 * * * * /home/liph/static.liph.ink/update-page.sh > /dev/null 2>&1
