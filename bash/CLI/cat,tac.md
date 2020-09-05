# cat, tac

## cat - 连接文件并在标准输出上输出

```shell
$ cat f - g # 依次输出文件 f，标准输入和文件 g 到标准输出
$ cat # 复制标准输入到标准输出
```

`cat [选项]... [文件]...`

将文件列表中的文件连接到标准输出。如果没有指定文件，或者指定文件为“-”，则从标准输入读取。

选项

```
-A, --show-all
       等价于 -vET

-b, --number-nonblank
       给非空输出行编号，使 -n 失效。

-e     等价于 -vE

-E, --show-ends
       在每行结束显示 $

-n, --number
       给所有输出行编号

-s, --squeeze-blank
       将所有的连续的多个空行替换为一个空行

-t     等价于 -vT

-T, --show-tabs
       把 TAB 字符显示为 ^I

-v, --show-nonprinting
       除了 LFD 和 TAB 之外的不可打印字符，用 ^ 和 M- 标记方式显示
```

## tac - 连接文件并逆序输出

```shell
$ cat 1.txt 
204
520
812
$ tac 1.txt 
812
520
204
$ tac 1.txt | tac
204
520
812
```