# shell脚本

## hello, world

```shell
#! /bin/bash
# shell script
echo "Hello, world."
```

第一行#! 开头，是脚本解释程序的声明指令，其后#后面的部分则是简单的注释，将其保存，先用chmod赋予其执行权，再执行这个shell

```shell
$ chmod 755 ./hello
$ ./hello
Hello, world.
```

## 定义与使用变量

```shell
#! /bin/bash
age=20
echo "age is ${age}."
```

给变量赋值的时候 = 号两边不能有空格，使用变量的时候在前面加$，用花括号告诉shell要打印的是age变量

## 整数运算

```shell
#! /bin/bash
i=1
i=$i+1
echo $i
```

执行脚本，输出的是 1+1 而不是 2，整数运算有以下几种方式

```shell
#! /bin/bash
i=1
i=`expr $i + 1`
echo $i
let "i=$i+1"
echo $i
((i=$i+1))
echo $i
i=$[$i+1]
echo $i
```

expr用于整数值运算，每一项用空格隔开；let表示数学运算；(())方式与let类似；$[]将中括号内的表达式作为数学运算先计算结果再输出

上面的shell输出2 3 4 5，其他运算还有+ - * / %等

## 流程控制

if

```shell
#! /bin/bash
# input a number
read x
if [ $x -gt 0 ]; then
 echo "1"
elif [ $x -eq 0 ];  then
 echo "0"
else
 echo "-1"
fi
```

test

`test expression or [ expression ] or [[ expression ]`

exit

exit 0 #退出脚本，状态值为0 （成功）

exit 1 #退出脚本，状态值为1 （失败

case

case表达式可以用来匹配一个给定的字符串，而不是数字

```shell
#!/bin/bash
echo "Please input "yes" or "no""
read var
case "$var" in
[yY][eE][sS] ) echo "Your input is YES" ;;
[nN][oO] ) echo "Your input is no" ;;
* ) echo "Input Error!" ;;
esac
exit 0
```

while 和 for

```shell
while ... ; do
......
done

for var in ... ; do
......
done
```

break用来跳出循环,continue跳过一个循环的余下部分

## 定义函数

```shell
#!/bin/bash
function show() {
    echo "hello , $1"
}
show world
```

函数需要先声明再使用，声明时function关键字可写可不写

函数中的$1 表示接收的第一个参数，如果有第二个参数则用$2 ，以此类推；$*表示接收的全部参数；$#用于记录参数的个数；$0代表脚本的文件名

## 基本输入输出

**echo**

显示字符串

echo "hello, world."

其中""可省略

echo hello, world.

转义字符

echo "\"hello, world.\""

显示换行与不换行

echo -e "hello, world\n" # 加上-e
echo -e "hello, world\c"

输出变量

echo $0
str="hello, world"
echo $str

单引号不取变量或转义

echo '\"$0\"' # 输出\"$0\"

反单引号输出命令执行结果

echo `date`

**read**

read命令接收标准输入，或其他文件描述符的输入。得到输入后，read命令将数据放入一个标准变量中

基本读取

read -p "Enter your name:" name
echo "hello $name"

-t选项指定read命令等待输入的秒数

read -t 5 -p "Enter your name:" name
echo "hello $name"

-n选项指定输入的字符

```shell
#!/bin/bash
read -n 1 -p "Input your answer:" a
case $a in
[Aa]) echo "your answer is A";;
[Bb]) echo "your answer is B";;
*) echo "error"
esac
exit 0
```

-s 输入不显示在监视器上

read -s -p "Input your password:" pass

利用read读取文件时，每次调用read命令都会读取文件中的“一行”文本。当文件没有可读的行时，read命令将以非零状态退出

cat file.txt | while read myline
do
echo "LINE:"$myline
done

## 格式化输出

`printf "Hello, world.\n"`

printf类似C语言中的库函数，用于格式化输出

```shell
printf "%-10s %5s\n" name class
printf "%-10s %5s\n"  Alice 1
printf "%-10s %5s\n" Bob 2
printf "%-10s %5s\n" Cindy 3
```

%-10s 指一个宽度为10个字符（-表示左对齐，没有则表示右对齐），任何字符都会被显示在10个字符宽的字符内，如果不足则自动以空格填充，超过也会将内容全部显示出来，输出如下

```shell
name       class
Alice          1
Bob            2
Cindy          3
```
