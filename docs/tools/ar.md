# ar

```shell
$ ar -r lib0.a 0.o 1.o  # 添加文件并创建归档
ar: 正在创建 lib0.a
$ ar -r lib0.a 2.o      # 继续添加
$ ar -t lib0.a          # 列出文件
0.o
1.o
2.o
$ ar -x lib0.a          # 解出文件
```

