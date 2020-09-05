# echo - 显示字符串

```shell
$ type echo
echo 是 shell 内建
$ echo "hello,  world."
hello,  world.
$ echo hello,  world
hello, world
$ echo $0
/bin/bash
$ echo $?
0
$ echo $$
4728
$ echo $HOME
/home/liph
$ echo `date +%F`
2020-07-13
```

选项

```
-n     不输出尾随的换行符

-e     启用解释反斜杠的转义功能

-E     禁用解释反斜杠的转义功能（默认）

--help 显示此帮助信息并退出

--version
       显示版本信息并退出

若 -e 可用，则以下序列即可识别：

\\     反斜线

\a     报警符(BEL)

\b     退格符

\c     禁止尾随的换行符

\e     escape 字符

\f     换页符

\n     另起一行

\r     回到行首

\t     水平制表符

\v     垂直制表符

\0NNN  字节数以八进制数 NNN (1至3位)表示

\xHH   字节数以十六进制数 HH (1至2位)表示

注意：您的 shell 可能内置了自己的 echo 程序版本，它会覆盖这里所提及的相应版本。请查阅您的 shell 文档获知它所支持的选项。
```
