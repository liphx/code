#!/bin/bash

set -ex

if [ ! -d ~/usr/bin/ ]; then
    mkdir -p ~/usr/bin/
fi
g++ -g -std=c++20 -W -Wall -Wfatal-errors tab2space.cpp -I ~/usr/include/ -o ~/usr/bin/tab2space
g++ -g -std=c++20 -W -Wall -Wfatal-errors password-generator.cpp -o ~/usr/bin/password-generator
g++ -g -std=c++20 -W -Wall -Wfatal-errors salary_calculator.cpp -o ~/usr/bin/salary_calculator
