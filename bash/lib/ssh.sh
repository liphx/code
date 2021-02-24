#!/bin/bash

ssh_login()
{
    username=$1
    ip=$2
    port=$3
    passwd=$4

    cmd="
        spawn ssh ${username}@${ip} -p ${port}
        expect { 
            \"password\" { send "${passwd}\\r" } 
        } 
        interact
    "
    expect -c "${cmd}"
}

# config file
# identifier username ip port passwd

config_file=$1
if [[ -z $config_file ]]; then
    echo "usage: $0 config_file"
    exit 1
fi

if [ ! -f ${config_file} ]; then
    echo "config file ${config_file} not exist"
    exit 2
fi

read -d "" -ra name_arr <<< $(awk '{print $1}' ${config_file})
read -d "" -ra user_arr <<< $(awk '{print $2}' ${config_file})
read -d "" -ra ip_arr <<< $(awk '{print $3}' ${config_file})
read -d "" -ra port_arr <<< $(awk '{print $4}' ${config_file})
read -d "" -ra passwd_arr <<< $(awk '{print $5}' ${config_file})

echo "select ssh connection:"
for i in ${!name_arr[@]}; do
    echo "  $((i + 1)). ${name_arr[$i]} - ${user_arr[$i]}@${ip_arr[$i]}:${port_arr[$i]}"
done
read select
let "select -= 1"
ssh_login ${user_arr[${select}]} ${ip_arr[${select}]} ${port_arr[${select}]} ${passwd_arr[${select}]}
