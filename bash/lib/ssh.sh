#!/bin/bash

ssh_login()
{
    username=$1
    ip=$2
    passwd=$3

    cmd="
        spawn ssh ${username}@${ip};
        expect { 
            \"password\" { send "${passwd}\\r" } 
        } 
        interact
    "
    expect -c "${cmd}"
}

ssh_login $@