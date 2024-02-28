#!/bin/bash

set -e

function info() {
    echo "INFO:" $@
}

function warning() {
    echo "WARNING:" $@
}

function error() {
    echo "ERROR:" $@
    exit 1
}

function main() {
    if [ $(id -u) -ne 0 ]; then
        error "should run as root"
    fi

    user_manage
    install_software
    start_server
}

function start_server() {
    nginx -s reload
    ufw allow http
    ufw allow https
    ufw status verbose
}

function add_user_if_not_exist() {
    user=$1
    if ! id $user > /dev/null 2>&1; then
        useradd -m -s /bin/bash $user
    else
        warning "User $user already exists!"
    fi
}

function user_manage() {
    add_user_if_not_exist liph
    add_user_if_not_exist jasmine
}

function install_software() {
    software_list=`cat package.list`
    info "Install software: $software_list"
    if which apt > /dev/null 2>&1; then
        sudo apt update
        sudo apt upgrade -y
        sudo apt install -y $software_list
    elif which yum > /dev/null 2>&1; then
        sudo yum install -y $software_list
    elif which zypper > /dev/null 2>&1; then
        sudo zypper install -y $software_list
    else
        error "Unrecognized package manager!"
    fi
}

main $@
exit 0
