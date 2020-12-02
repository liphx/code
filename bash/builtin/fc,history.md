# History

`fc, history`

## fc

`fc [-e ename] [-lnr] [first] [last]`

从历史列表中列出或者重新编辑并执行命令。
FIRST 和 LAST 变量可以是数字用于指定范围，或者 FIRST 可以是字符串，意味着以这个字符串打头的最近的一个命令

+ -e 编辑器， 如果不指定则为 `${FCEDIT:-${EDITOR:-vi}}`
+ -l 列出行而不编辑
+ -n 列举时省略行号
+ -r 反转行的顺序(最新行在前)

`fc -s [pat=rep] [command]`

命令会在替换之后被重新执行

```shell
$ sduo apt update
bash: sduo：未找到命令
$ fc -s sduo=sudo
sudo apt update
```

## history

`history [n]` 列出最后n行历史命令，每个被修改的条目加上 `*` 前缀

`history -c` 删除所有条目从而清空历史列表

```shell
$ history -c
$ history 
    1  history 
```

`history -d offset` 从指定位置删除历史列表。负偏移量将从历史条目末尾开始计数

`history -d start-end` 删除start 与 end 之间的历史列表

`history [-anrw] [filename]`

+ -a    将当前会话的历史行追加到历史文件中
+ -n    从历史文件中读取所有未被读取的行并且将它们附加到历史列表
+ -r    读取历史文件并将内容追加到历史列表中
+ -w    将当前历史写入到历史文件中

`history -ps arg`

+ -p 对每一个 ARG 参数展开历史并显示结果，而不存储到历史列表中
+ -s 以单条记录追加 ARG 到历史列表中