# Shell 扩展

Shell有7种扩展，扩展的顺序是大括号扩展；波浪号扩展；参数和变量扩展；算术扩展；命令替换；单词拆分；文件名扩展。如果系统支持，还有一种扩展：进程替换，它和波浪号、参数、变量、算术扩展以及命令替换是同时进行的。

## 大括号扩展

```shell
$ echo a{a,b,c}
aa ab ac
$ echo 0{1..9}
01 02 03 04 05 06 07 08 09
$ echo 0{1..9..3}
01 04 07
```

## 波浪号扩展

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

## 参数扩展

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

## 命令替换

```
$(cmd) or `cmd`
```

## 算术扩展

## 进程替换

## 单词拆分

## 文件名扩展