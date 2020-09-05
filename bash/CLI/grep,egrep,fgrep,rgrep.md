# grep - globally search a regular expression and print

`grep, egrep, fgrep, rgrep`

```shell
$ grep error *.log          # 在*.log中查找包含字符串error的行
$ ps -ef | grep nginx       # 可以接受标准输入
$ grep error *.log -i       # 忽略大小写搜索
$ grep error *.log -c       # 只统计行数
$ grep *.log -f txt         # 从文件中读取PATTERN
$ grep error *.log -n       # 前导行号加冒号
$ grep error *.log -v       # 显示不匹配的行
$ grep error *.log -o       # 只输出匹配的部分，如果一行不止匹配一个会输出多个
$ grep error *.log -C num   # 列出上下文
$ grep ^error$ *.log        # 正则表达式，只包含error的一行
$ grep error -r             # 在当前目录递归查找
$ grep error logfile -r     # 在指定目录下递归查找
```

其他

`egrep` 等同`grep -E`，extended，支持更多的正则表达式元字符  
`fgrep` 等同`grep -F`，fixed，把所有的字母都看作单词，不做正则表达式处理  
`rgrep` 等同`grep -r`
