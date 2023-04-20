[toc]

# bash - Bourne Again Shell

## 选项

+ `-c` 命令来自第一个非选项参数，如果命令字符串后面还有参数，第一个参数赋值给$0，后面的参数依次排序，例如执行`bash -c 'echo $@' 1 2 3 4`的输出为`2 3 4`
+ `-i` interactive，交互式
+ `-l` 或 `--login` 让bash的行为类似于登录shell
+ `-r` 受限的bash
+ `-s` 如果有 -s 选项，或者如果选项处理完以后，没有参数剩余，那么命令将从标准输入读取。 这个选项允许在启动一个交互 shell 时可以设置位置参数。
+ `-v` shell读取的时候打印输入行
+ `-x` 执行的时候打印命令及参数
+ `-D` 向标准输出打印一个以 $ 为前导的，以双引号引用的字符串列表。 这是在当前语言环境不是 C 或 POSIX 时，脚本中需要翻译的字符串。 这个选项隐含了 -n 选项；不会执行命令
+ `[-+]O [shopt_option]` `shopt_option` 是一个 shopt 内建命令可接受的选项 (参见下面的 shell 内建命令(SHELL BUILTIN COMMANDS) 章节)。 如果有 `shopt_option`，-O 将设置那个选项的取值； +O 取消它。 如果没有给出 `shopt_option`，shopt 将在标准输出上打印设为允许的选项的名称和值。 如果启动选项是 +O，输出将以一种可以重用为输入的格式显示。
+ `--` -- 标志选项的结束，禁止其余的选项处理。任何 -- 之后的参数将作为文件名和参数对待。参数 - 与此等价
+ 内建命令`set`中的单字符选项也可以使用

## 参数

没有指定 -c 或 -s 选项下，第一个参数将假定为一个包含 shell 命令的文件的名字。 如果 bash 是以这种方式启动的， $0 将设置为这个文件的名字，位置参数将设置为剩余的其他参数。 Bash 从这个文件中读取并执行命令，然后退出。 Bash 的退出状态是脚本中执行的最后一个命令的退出状态。 如果没有执行命令，退出状态是0。 尝试的步骤是先试图打开在当前目录中的这个文件，接下来， 如果没有找到，shell 将搜索脚本的 PATH 环境变量中的路径

## builtins

### :

`: [arguments]`

扩展参数，执行重定向外什么也不做，退出状态为0

### .

`. filename [arguments]`

在当前shell上下文中读取和执行的filename中的命令。退出状态是最后执行的命令的退出状态；如果未执行任何命令，则返回零。如果找不到文件名或无法读取文件名，则返回状态为非零。此内置命令等价于`source`

### break

`break [n]`

从`for`, `while`, `until` 或 `select`中退出。 如果提供了n，则退出第n个循环。n必须大于或等于1。返回状态为零，除非n不是大于或等于1。

### continue

`continue [n]`

继续循环的下一次迭代，其它同`break`
## IFS

IFS的默认值为空白字符（换行符、制表符或者空格）。

## alias

```shell
$ alias             # 列出所有别名
$ alias -p          # 同上
$ alias ll='ls -l'  # 创建/覆盖别名
$ alias ll          # 打印别名的名称和值
alias ll='ls -l'
$ unalias ll        # 取消别名
$ alias ll
bash: alias: ll: not found
$ alias ll=
$ alias ll
alias ll=''
$ alias ls='ls -l'
$ \ls               # 忽略别名, 执行原本的命令
```

## arithmetic

+ `(( expression ))`
+ `let "expression"`

如果表达式的值非零，则返回状态为0;否则返回状态为1

```shell
i=0
((i++))
echo $i         # 1
((i = i + 1))
echo $i         # 2
let "i = i * 5"
echo $i         # 10
```

## 数组

### 普通数组

```shell
#!/bin/bash

array_var=(test1 test2 test3 test4) # 这些值将会存储在以0为起始索引的连续位置上
echo ${array_var[0]}                # test1

array_var[0]=ttt
echo ${array_var[0]}                # ttt

i=3
echo ${array_var[$i]}               # test4

array_var[4]=test5
echo ${array_var[*]}                # ttt test2 test3 test4 test5

array_var[10]=test6
echo ${array_var[*]}                # ttt test2 test3 test4 test5 test6
echo ${array_var[@]}                # ttt test2 test3 test4 test5 test6
echo ${#array_var[*]}               # 6
echo ${!array_var[*]}               # 0 1 2 3 4 10

array_var[-1]=test7
echo ${array_var[*]}                # ttt test2 test3 test4 test5 test7
echo ${!array_var[*]}               # 0 1 2 3 4 10

array_var[-2]=test8
echo ${array_var[*]}                # ttt test2 test3 test4 test5 test8 test7
echo ${!array_var[*]}               # 0 1 2 3 4 9 10
```

### 关联数组

首先，需要使用声明语句将一个变量定义为关联数组

```shell
#!/bin/bash

declare -A ass_array                # 使用声明语句将一个变量定义为关联数组

ass_array=([index1]=val1 [index2]=val2)
echo ${ass_array[index1]}           # val1

ass_array[index3]=val3
echo ${ass_array[index3]}           # val3

echo ${#ass_array[*]}               # 3
echo ${ass_array[*]}                # val1 val3 val2
echo ${!ass_array[*]}               # index1 index3 index2
```

## Job Control

`bg, fg, jobs, kill, wait, disown, suspend`

```shell
$ ./main &
[1] 32598
$ jobs
[1]+  运行中               ./main &
$ ./main
^Z
[2]+  已停止               ./main
$ jobs
[1]-  运行中               ./main &
[2]+  已停止               ./main
$ bg %2
[2]+ ./main &
$ jobs
[1]-  运行中               ./main &
[2]+  运行中               ./main &
$ fg %2
./main
^C
$ kill %1
$
[1]+  已终止               ./main
```

```shell
$ ./main &
[1] 32679
$ disown %1
$ jobs
$ pgrep main
32679
```

```shell
$ kill -l
 1) SIGHUP  2) SIGINT  3) SIGQUIT  4) SIGILL  5) SIGTRAP
 6) SIGABRT  7) SIGBUS  8) SIGFPE  9) SIGKILL 10) SIGUSR1
11) SIGSEGV 12) SIGUSR2 13) SIGPIPE 14) SIGALRM 15) SIGTERM
16) SIGSTKFLT 17) SIGCHLD 18) SIGCONT 19) SIGSTOP 20) SIGTSTP
21) SIGTTIN 22) SIGTTOU 23) SIGURG 24) SIGXCPU 25) SIGXFSZ
26) SIGVTALRM 27) SIGPROF 28) SIGWINCH 29) SIGIO 30) SIGPWR
31) SIGSYS 34) SIGRTMIN 35) SIGRTMIN+1 36) SIGRTMIN+2 37) SIGRTMIN+3
38) SIGRTMIN+4 39) SIGRTMIN+5 40) SIGRTMIN+6 41) SIGRTMIN+7 42) SIGRTMIN+8
43) SIGRTMIN+9 44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12
53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9 56) SIGRTMAX-8 57) SIGRTMAX-7
58) SIGRTMAX-6 59) SIGRTMAX-5 60) SIGRTMAX-4 61) SIGRTMAX-3 62) SIGRTMAX-2
63) SIGRTMAX-1 64) SIGRTMAX
```

## 内建命令 builtin

+ 内建命令不需要使用子进程来执行，包括`cd`, `exit`, `type`, `alias`, `.`, `echo`, `history`等

## case

```shell
case word in
    [ [(] pattern [| pattern]…) command-list ;;]…
esac
```

```shell
read ans
case $ans in
    y | Y | yes | YES)
    echo 'YES'
    ;;
    n | N | no | NO)
    echo 'NO'
    ;;
    *)
    echo '??'
    ;;
esac
```

每个case子句以`;;`， `;&` 或 `;;&`结束

+ `;;`     在第一次模式匹配之后不尝试后续匹配
+ `;&`     在第一次模式匹配之后继续执行下一条子句
+ `;;&`     在第一次模式匹配之后继续匹配，并在匹配的子句处往下执行

```shell
read ans
case $ans in
    1)
    echo 1;
    ;&          # 输入1，输出为1 2
    2)
    echo 2;     # 输入2，输出为2
    ;;
    3)
    echo 3;     # 输入3，输出为3 ??
    ;;&
    *)
    echo '??'
    ;;
esac
```

类比C语言的`switch`， `;;` 即 C语言中的`break`， `;&`为不加`break`的分支，`;;&`则再进行一次匹配

## 退出状态

如果没有匹配项，退出状态为0，否则为执行命令的退出状态

## echo - 显示字符串

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
-E     禁用解释反斜杠的转义功能（默认）
-e     启用解释反斜杠的转义功能
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
```

## exec

exec命令创建全新的文件描述符

```
exec 3<input.txt    #使用文件描述符3打开并读取文件
exec 4>output.txt   #打开文件进行写入
exec 5>>input.txt
```

## Shell 扩展

Shell有7种扩展，扩展的顺序是大括号扩展；波浪号扩展；参数和变量扩展；算术扩展；命令替换；单词拆分；文件名扩展。如果系统支持，还有一种扩展：进程替换，它和波浪号、参数、变量、算术扩展以及命令替换是同时进行的。

### 大括号扩展

```shell
$ echo a{a,b,c}
aa ab ac
$ echo 0{1..9}
01 02 03 04 05 06 07 08 09
$ echo 0{1..9..3}
01 04 07
```

### 波浪号扩展

```
~           /home/liph
~liph       /home/liph
~root       /root
~/tmp       /home/liph/tmp
~+          $PWD
~-          $OLDPWD
~N          dirs +N
~+N         dirs +N
~-N         dirs -N
```

### 参数扩展

```shell
# ${parameter:−word}    如果参数没有设置或为空，替换为单词；否则为参数
# ${parameter:=word}    如果参数没有设置或为空，把单词赋值给参数
# ${parameter:?word}    如果参数没有设置或为空，把单词写到标准错误，退出shell
# ${parameter:+word}    如果参数没有设置或为空，不进行任何替换；否则替换为单词
# ${parameter:offset}   参数:偏移量
# ${parameter:offset:length} 参数:偏移量:长度，如果长度是负数，则表示结尾的偏移量

str='hello, world'
echo ${#str}            # 12, 长度
echo ${str:0:${#str}}   # hello, world
echo ${str:1}           # ello, world
echo ${str:1:-1}        # ello, worl
echo ${str:-1}          # hello, world

echo ${var}             #
echo ${var:-abc}        # abc
echo ${var}             #
echo ${var:=abc}        # abc
echo ${var}             # abc
echo ${var:=bcd}        # abc

#echo ${new:?bcd}
echo ${var:?bcd}        # abc

echo ${var2:+bcd}
echo ${var:+bcd}        # bcd
echo ${var}             # abc
```

```shell
# ${!prefix*}
# ${!prefix@} 扩展为名称中含有前缀的变量，以IFS的第一个字符分隔。如果使用了@，并且在双引号内扩展，则每个变量都扩展成单独的单词
var1=hello
var2=world
echo ${!var*}           # var1 var2

# ${!name[@]}
# ${!name[*]} 如果名称是个数组，扩展成数组下标或者key列表
array=(a b c)
echo ${!array[*]}       # 0 1 2
array[100]=xyz
echo ${!array[*]}       # 0 1 2 100

# ${#parameter} 参数扩展后的字符数，如果是带下标*/@的数组名，则为数组中的元素个数
echo ${#var1}           # 5
echo ${#array[*]}       # 4
echo ${#array}          # 1

# ${parameter#word}
# ${parameter##word} 单词被扩展成一个模式，如果模式匹配参数扩展后的开始部分，则替换的结果是该模式最短(#)或最长(##)匹配参数扩展的部分被删除后的字符串
var='a1a1a1'
echo ${var#a*a}       # 1a1
echo ${var##a*a}      # 1

# ${parameter%word}
# ${parameter%%word} 同上，从后匹配
echo ${var%1*1}       # a1a
echo ${var%%1*1}      # a

# ${parameter/pattern/string}
var=0a1a
echo ${var/a/A}     # 0A1a 替换第一个
echo ${var//a/A}    # 0A1A 全替换
echo ${var/#a/A}    # 0a1a 替换开头的第一个
echo ${var/%a/A}    # 0a1A 替换结尾的第一个
echo ${var/a/}      # 01a

# ${parameter^pattern}
# ${parameter^^pattern}
# ${parameter,pattern}
# ${parameter,,pattern} 大小写替换
var=abCD
echo ${var^*}   # AbCD
echo ${var^^*}  # ABCD
echo ${var,*}   # abCD, 把a替换小写，无变化
echo ${var,,*}  # abcd

# ${parameter@operator} Q E P A a
var=abc\ d
echo ${var@Q}   # 'abc d', 即quote
echo ${var@E}   # abc d, 即escape
var='\u'
echo ${var@P}   # liph, 即prompt, 按PS1方式展开
echo ${var@A}   # var='\u', 即assignment
readonly var
echo ${var@A}   # declare -r var='\u'
echo ${var@a}   # r, 即attribute，获取属性
```

### 命令替换

```
$(cmd) or `cmd`
```

### 算术扩展

```
$(( exp ))
```

### 进程替换

```
<(list)    or    >(list)

'<' 或 '>' 与左边括号之间不能有空格，否则会被解释成重定向
```

## History

`fc, history`

### fc

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

### history

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

## 函数

### 函数定义

+ `name () compound-command [ redirections ]`
+ `function name [()] compound-command [ redirections ]`

复合命令`(Compound Commands)`包括循环，条件与组分组命令`(Grouping Commands)`

### 退出状态

函数定义的退出状态为零，除非出现语法错误或已存在同名的只读函数。在执行时，如果给定一个数值参数返回`(return)`，这是函数的返回状态；否则，函数的返回状态是返回前执行的最后一个命令的退出状态
。

### 调用函数

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

## Grouping Commands

`( list )`

在圆括号之间放置一列命令将创建一个子shell环境，列表中的每个命令将在该子shell中执行。由于该列表是在子shell中执行的，所以在子shell完成后，变量分配将不再有效。

`{ list; }`

在花括号之间放置一个命令列表将导致该列表在当前shell上下文中执行。不创建子shell。必须使用列表后面的分号(或换行符)。

此外，大括号是保留字，因此必须用空格或其他shell元字符将它们与列表分隔开。括号是操作符，即使它们与列表之间没有空格，shell也会将它们识别为单独的标记。这两个构造的退出状态都是list的退出状态。

## 比较与测试

## if

用法

```shell
if condition;
then
    commands;
fi
```

以及

```shell
if condition;
then
    commands;
elif condition;
then
    commands;
else
    commands;
fi
```

### 算术比较

比较条件通常被放置在封闭的中括号内。一定要注意在[ 或 ]与操作数之间有一个空格。如果忘记了这个空格，脚本就会报错。

对变量或值进行算术条件测试：

`[ $var -eq 0 ]`  当$var等于0时，返回真
`[ $var -ne 0 ]`  当$var不为0时，返回真

其他重要的操作符如下。

```
-gt：大于。
-lt：小于。
-ge：大于或等于。
-le：小于或等于。
```

-a是逻辑与操作符，-o是逻辑或操作符。可以按照下面的方法结合多个条件进行测试：`[ $var1 -ne 0 -a $var2 -gt 2 ]` 以及 `[ $var1 -ne 0 -o $var2 -gt 2 ]`

### 文件系统相关测试

```
[ -f $file_var ]：如果给定的变量包含正常的文件路径或文件名，则返回真。
[ -x $var ]：如果给定的变量包含的文件可执行，则返回真。
[ -d $var ]：如果给定的变量包含的是目录，则返回真。
[ -e $var ]：如果给定的变量包含的文件存在，则返回真。
[ -c $var ]：如果给定的变量包含的是一个字符设备文件的路径，则返回真。
[ -b $var ]：如果给定的变量包含的是一个块设备文件的路径，则返回真。
[ -w $var ]：如果给定的变量包含的文件可写，则返回真。
[ -r $var ]：如果给定的变量包含的文件可读，则返回真。
[ -L $var ]：如果给定的变量包含的是一个符号链接，则返回真。
```

### 字符串比较

进行字符串比较时，最好用双中括号，因为有时候采用单个中括号会产生错误。双中括号是Bash的一个扩展特性。如果出于性能考虑，使用ash或dash来运行脚本，那么将无法使用该特性。

```
[[ $str1 = $str2 ]]：当str1等于str2时，返回真。也就是说，str1和str2包含的文本是一模一样的。
[[ $str1 == $str2 ]]：这是检查字符串是否相同的另一种写法。测试两个字符串是否不同。
[[ $str1 != $str2 ]]：如果str1和str2不相同，则返回真。
[[ $str1 > $str2 ]]：如果str1的字母序比str2大，则返回真。
[[ $str1 < $str2 ]]：如果str1的字母序比str2小，则返回真。
[[ -z $str1 ]]：如果str1为空串，则返回真。
[[ -n $str1 ]]：如果str1不为空串，则返回真。
```

## test

test命令可以用来测试条件。用test可以避免使用过多的括号，增强代码的可读性。[]中的测试条件同样可以用于test命令。

`if  [ $var -eq 0 ]; then echo "True"; fi` 可以写`if  test $var -eq 0 ; then echo "True"; fi`

test是一个外部程序，需要衍生出对应的进程，而 [ 是Bash的一个内部函数，因此后者的执行效率更高。test兼容于Bourne shell、ash、dash等。

# Lists of Commands

```shell
cmd1 && cmd2    # 当且仅当cmd1退出状态为0才执行cmd2
cmd1 || cmd2    # 当且仅当cmd1退出状态不为0才执行cmd2
cmd1 ; cmd2     # 按顺序执行cmd1，cmd2
cmd &           # 后台异步执行cmd
```

优先级 && 和 || 相同，优于优先级相同的 ; 和 &

## 循环

## for

`for name [ [in [lists …] ] ; ] do commands; done`
`for (( expr1 ; expr2 ; expr3 )) ; do commands ; done`

### 面向列表的for循环

```shell
for var in list;
do
    commands;
done
```

list可以是一个字符串，也可以是一个值序列

值序列 `{A..Z}`, `{0..9} {a..z}`

```shell
$ echo {A..Z}
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
$ echo {a..z} {0..9}
a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9
```

如果不指定`in lists`，默认为`in $@`

### 迭代指定范围的数字

```shell
for((i=0; i<10; i++))
{
    commands;
}
```

## while

`while test-commands; do consequent-commands; done`

```shell
while true;
do
    commands;
done
```

用true作为循环条件能够产生无限循环

## until

`until test-commands; do consequent-commands; done`

until会一直循环，直到给定的条件为真。

```shell
x=0;
until [ $x -eq 9 ];
do
    let x++;
    echo $x;
done
```

无限循环 `while true; do cmd; done` 等价于 `until false; do cmd; done`

三种循环都可以使用`break`，`continue`来控制

## pipeline

```shell
cmd1 | cmd2     # cmd1的标准输出成为cmd2的标准输入
cmd1 |& cmd2    # cmd1的标准输出、标准错误成为cmd2的标准输入，类似于cmd1 2>&1 | cmd2
```

未开启`pipefail`选项下，管道的退出状态是最后一条命令的退出状态

## pwd - print work directory

输出当前工作目录的完整名称

```shell
$ type -a pwd
pwd 是 shell 内建
pwd 是 /usr/bin/pwd
pwd 是 /bin/pwd
$ pwd
/home/liph
```

## return

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

## select

`select name [in words …]; do commands; done`

如果省略`in words`，则会指定`in $@`

创建菜单供用户选择

```shell
echo "select your linux distribution"
linux="Debian Centos Ubuntu Arch"
select os in $linux:
do
    echo "your select is ${REPLY}, $os is the best!"
    break;
done
```

```shell
$ bash select.sh
select your linux distribution
1) Debian
2) Centos
3) Ubuntu
4) Arch:
#? 1
your select is 1, Debian is the best!
```

## set

```shell
set [--abefhkmnptuvxBCEHPT] [-o option-name] [argument …]
set [+abefhkmnptuvxBCEHPT] [+o option-name] [argument …]
```

如果不指定参数，`set`会打印出所有变量与函数

选项

```
-e  如果命令退出状态非0，立即退出shell
-x  执行命令前先打印（先扩展变量`PS4`后打印命令）
+x  禁止调试
-v  当命令进行读取时显示输入
+v  禁止打印输入
-u  视未设置的变量和参数（特殊参数@或*除外）为错误，在标准错误写入错误信息，并退出非交互运行的shell
-o
    pipefail  只要管道中有任何一个命令返回非0，则整个管道操作被视为失败
```

使用选项-x，启用shell脚本的跟踪调试功能：`$ bash -x script.sh`

使用set -x和set +x对脚本进行部分调试

```shell
#!/bin/bash
for i in {1..6};
do
    set -x
    echo $i
    set +x
done
```

把shebang从#!/bin/bash改成#!/bin/bash -xv，这样一来，不用任何其他选项就可以启用调试功能了

如果使用了-x -v或set -x -v，调试输出会被发送到stderr

## shell styleguide

参考

<https://google.github.io/styleguide/shellguide.html>

## time

`time [-p] command [arguments...]`

-p POSIX格式输出

time的输出

+ real      时钟时间
+ user      用户态CPU时间
+ sys       内核态CPU时间

## type

```shell
$ type -t type
builtin
$ type ls
ls 是“ls --color=auto”的别名
$ type -a ls
ls 是“ls --color=auto”的别名
ls 是 /usr/bin/ls
ls 是 /bin/ls
$ type mkcd
mkcd 是函数
mkcd ()
{
    dir=$1;
    mkdir -p "${dir}";
    cd "${dir}"
}
$ type -f mkcd # 抑制 shell 函数查询
bash: type: mkcd：未找到
$ type -P ls
/usr/bin/ls
$ type -t ls
alias
$ type -p ls # 当 -t 不返回 file时，无任何值
```

## 变量

+ 变量名由一系列字母、数字和下划线组成，其中不包含空白字符，变量包含一个值与0或多个属性，属性由`declare`来声明。
+ 常用的惯例是在脚本中使用大写字母命名环境变量，使用驼峰命名法或小写字母命名其他变量。
+ 赋值操作：`name=[value]`，其中，空字符串是一个有效值
+ 如果value不包含任何空白字符（例如空格），那么就不需要将其放入引号中，否则必须使用单引号或双引号
+ `var = value`不同于`var=value`。两边没有空格的等号是赋值操作符，加上空格的等号表示的是等量关系测试。
+ 在变量名之前加上美元符号（$）就可以访问变量的内容
+ `echo $var` 也可以这样写：`echo ${var}`
+ 使用单引号时，变量不会被扩展，仍依照原样显示。这意味着`echo '$var'`会显示`$var`。
+ 但如果变量var已经定义过，那么`echo "$var"`会显示出该变量的值；如果没有定义过，则什么都不显示。
+ 可以用下面的方法获得变量值的长度：`length=${#var}`

## 位置参数

位置参数是由一个或多个数字表示的参数，位置参数是在调用shell时从参数中分配的，可以使用`set`重新分配。位置参数N可以引用为 `${N}`，或者当N为单个数字时引用为 `$N`。位置参数不能用赋值语句赋值。`set`和`shift`用于设置和取消设置。在执行函数时它们被临时替换。

## 特殊参数

+ `$*`    扩展为从1开始的位置参数，当展开不在双引号内时，每个位置参数展开为一个单独的单词；当在双引号内进行扩展时，它将扩展为一个单词，每个参数的值由`IFS`特殊变量的第一个字符分隔，即`"$*"` 等价为`"$1c$2c..."`，其中c是`IFS`变量值的第一个字符。 如果未设置`IFS`，则参数之间用空格分隔。 如果`IFS`为null，则参数连接时不插入分隔符
+ `$@`    扩展为从1开始的位置参数。`"$@"` 等价为`"$1" "$2" ...`
+ `$#`    位置参数个数
+ `$?`    最近执行的前台管道的退出状态
+ `$-`    `set`或由shell本身设置的那些选项标志（如`-i`选项）
+ `$$`    shell的进程ID。在子shell中，它扩展为调用shell的进程ID，而不是子shell的
+ `$!`    最近放置到后台的作业的进程ID，无论是作为异步命令执行还是使用`bg`
+ `$0`    shell或shell脚本的名称，这是在shell初始化时设置的。如果使用Bash执行文件，则将`$0` 设置为该文件的名称；如果Bash以`-c`选项启动，则将`$0`设置为要执行的字符串之后的第一个参数（如果存在）；否则，将其设置为用于调用Bash的文件名
+ `$_`    在shell启动时，设置为shell或shell脚本的绝对路径名；随后，扩展到前台执行的前一个简单命令的最后一个参数

## shift

`shift [n]`

从 n+1 ... 开始的选项被重命名为 $1 ....  从 $# 向下直到 $#-n+1 的选项被取消定义。 n  必须是非负整数，小于或等于  $#。如果  n  是
0，不会改变参数。如果没有给出 n， 就假定它是 1。如果 n 比 $# 大，位置参数不会改变。返回值大于 0，如果 n 比 $# 大或小于 0；否则返
回 0。

