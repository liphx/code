# tee - 从标准输入读入并写往标准输出和文件

```shell
$ echo hello | tee out.txt
hello
$ cat out.txt 
hello
```

tee只能从stdin中读取

默认情况下，tee命令会将文件覆盖，但它提供了一个-a选项，可用于追加内容
