# type

```shell
$ type -t type
builtin
$ type ls
ls 是“ls --color=auto”的别名
$ type -a ls
ls 是“ls --color=auto”的别名
ls 是 /usr/bin/ls
ls 是 /bin/ls
$ type mkcd 
mkcd 是函数
mkcd () 
{ 
    dir=$1;
    mkdir -p "${dir}";
    cd "${dir}"
}
$ type -f mkcd # 抑制 shell 函数查询
bash: type: mkcd：未找到
$ type -P ls 
/usr/bin/ls
$ type -t ls
alias
$ type -p ls # 当 -t 不返回 file时，无任何值
```