# readlink

```shell
$ readlink /usr/bin/vi
/etc/alternatives/vi
$ readlink /usr/bin/vi -f
/usr/bin/vim.basic
```

+ `-f` 递归跟随给出文件名的所有符号链接以标准化；所有组件都必须存在
+ `-e` 递归跟随给出文件名的所有符号链接以标准化，除最后一个外所有组件必须存在
+ `-m` 递归跟随给出文件名的所有符号链接以标准化；各个组件没有必须存在的要求