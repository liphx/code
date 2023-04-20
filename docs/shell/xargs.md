# xargs - 从标准输入重建并执行命令行

```shell
$ ls *.c | xargs grep main
$ cat example.txt    # 样例文件
1 2 3 4 5 6
7 8 9 10
11 12
$ cat example.txt | xargs # xargs默认会执行echo命令
1 2 3 4 5 6 7 8 9 10 11 12
$ cat example.txt | xargs -n 3 # -n选项可以限制每次调用命令时用到的参数个数
1 2 3
4 5 6
7 8 9
10 11 12
$ find . -iname '*.docx' -print0 | xargs -0 grep -L image
$ find . -type f -name "*.txt" -print0 | xargs -0 rm -f
$ find . -type f -name "*.c" -print0 | xargs -0 wc –l
$ cat args.txt | xargs -I {} cmd -1 {} -2
```

`xargs [选项]... 命令 [初始参数]...`

xargs命令接受来自stdin的输入，将数据解析成单个元素，然后调用指定命令并将这些元素作为该命令的参数。xargs默认使用空白字符分割输入并执行/bin/echo。

xargs会将参数放置在指定命令的尾部

-I {}指定了替换字符串。为该命令提供的各个参数会通过stdin读取并依次替换掉字符串{}。

使用-I的时候，命令以循环的方式执行。如果有3个参数，那么命令就会连同{}一起被执行3次。{}会在每次执行中被替换为相应的参数。
