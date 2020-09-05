# case

## 语法

```shell
case word in
    [ [(] pattern [| pattern]…) command-list ;;]…
esac
```

## 示例

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

+ `;; `     在第一次模式匹配之后不尝试后续匹配
+ `;& `     在第一次模式匹配之后继续执行下一条子句
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
