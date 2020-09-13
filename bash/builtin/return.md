# return 

`return [n]`

用来退出函数并指定退出状态，如果没用指定n，退出状态为函数执行的最后一条命令的退出状态

```shell
$ return
bash: return: 只能从函数或者源脚本返回(`return')
$ cat test.sh 
return
$ bash test.sh 
test.sh: 第 1 行：return: 只能从函数或者源脚本返回(`return')
$ source test.sh
```

在终端中输入`return`，得到提示`return`只能在函数或由 `.` 和`source`命令执行的脚本中使用

