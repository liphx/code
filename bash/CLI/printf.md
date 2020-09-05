# printf - 格式化打印

`printf [-v var] FORMAT [ARGUMENTS]` 在 FORMAT 的控制下格式化并打印 ARGUMENTS 参数

-v var	将输出赋值给 shell 变量 VAR 而不显示在标准输出上

格式以C语言printf函数的方式控制输出

```shell
$ type printf 
printf 是 shell 内建
$ printf "%s\n" "hello, world"
hello, world
$ printf "%.3f" 3.14
3.140
$ printf -v tmp %s hello
$ echo $tmp
hello
```
