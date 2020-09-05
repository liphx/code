# 循环

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
