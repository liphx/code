# 函数

## 函数定义

+ `name () compound-command [ redirections ]`
+ `function name [()] compound-command [ redirections ]`

复合命令`(Compound Commands)`包括循环，条件与组分组命令`(Grouping Commands)`

## 退出状态

函数定义的退出状态为零，除非出现语法错误或已存在同名的只读函数。在执行时，如果给定一个数值参数返回`(return)`，这是函数的返回状态；否则，函数的返回状态是返回前执行的最后一个命令的退出状态
。

## 调用函数

```shell
fname; # 使用函数名执行函数
fname arg1 arg2; # 传递参数
```

```shell
function func()
{
    echo $1;
    echo "$@";
    return 0;
}
func hello world;
```

输出

```
hello
hello world
```

其中

```
$0是脚本名称。
$1是第一个参数。
$2是第二个参数。
$n是第n个参数。
"$@"被扩展成"$1""$2""$3"等。
"$*"被扩展成"$1c$2c$3"，其中c是IFS的第一个字符。
"$@"要比"$*"用得多。由于"$*"将所有的参数当作单个字符串，因此它很少被使用。
```

## 递归

```shell
function func()
{
    echo hello;
    sleep 1;
    func;
}
func;
```

bash也支持递归函数，自身调用自己（或循环调用）

```shell
:(){ :|:& };: # fork 炸弹，":" 可作为函数名
```

## 变量

使用`local`定义变量将仅对函数及其调用的命令可见

```shell
func1()
{
    local var='func1 local'
    func2
}

func2()
{
    echo "In func2, var = $var"
}

var=global
func1           # => In func2, var = func1 local
```
