#!/bin/bash

cd $HOME
git clone https://github.com/liphx/static.liph.ink.git
cd static.liph.ink
python3 build.py
cd $HOME
mkdir -p www/resource
