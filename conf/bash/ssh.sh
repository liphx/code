#!/bin/bash

# config file
# identifier username host/ip port passwd

config_file=$1

if [[ -z $config_file ]]; then
    echo "usage: $0 config_file"
    exit 1
fi

if [ ! -f ${config_file} ]; then
    echo "config file ${config_file} does not exist"
    exit 2
fi

if ! which expect > /dev/null 2>&1; then
    echo "expect: command not found"
    exit 3
fi

start_with() {
    if [[ $1 =~ ^$2.* ]]; then
        true
    else
        false
    fi
}

ssh_login() {
    username=$1
    ip=$2
    port=$3
    passwd=$4

    cmd="
        set timeout 2
        spawn ssh ${username}@${ip} -p ${port}
        expect {
            \"Connection refused\" exit
            \"Name or service not known\" exit
            \"continue connecting\" { send \"yes\\r\"; exp_continue }
            \"password:\" { send \"${passwd}\\r\" }
            \"Password:\" { send \"${passwd}\\r\" }
        }
        interact
    "
    expect -c "${cmd}"
}

read -d "" -ra name_arr     <<< $(grep -v '^#' ${config_file} | awk '{print $1}')
read -d "" -ra user_arr     <<< $(grep -v '^#' ${config_file} | awk '{print $2}')
read -d "" -ra ip_arr       <<< $(grep -v '^#' ${config_file} | awk '{print $3}')
read -d "" -ra port_arr     <<< $(grep -v '^#' ${config_file} | awk '{print $4}')
read -d "" -ra passwd_arr   <<< $(grep -v '^#' ${config_file} | awk '{print $5}')

if [[ -z $2 ]]; then
    echo "ssh information:"
    printf "  %-6s%-16s%-32s%s\n" id identifier username@ip port
    for i in ${!name_arr[@]}; do
        printf "  %-6s%-16s%-32s%s\n" "$((i + 1))" "${name_arr[$i]}" "${user_arr[$i]}@${ip_arr[$i]}" "${port_arr[$i]}"
    done
    printf "input id or identifier: "
    read select
else
    select=$2
fi

if [[ $select =~ ^[1-9][0-9]* ]]; then
    let "select -= 1"
    ssh_login ${user_arr[${select}]} ${ip_arr[${select}]} ${port_arr[${select}]} ${passwd_arr[${select}]}
else
    match=no
    for i in ${!name_arr[@]}; do
        if start_with ${name_arr[$i]} ${select}; then
            match=yes
            ssh_login ${user_arr[$i]} ${ip_arr[$i]} ${port_arr[$i]} ${passwd_arr[$i]}
            break
        fi
    done
    if [[ $match = "no" ]]; then
        echo "no connection match $select"
    fi
fi

exit 0
