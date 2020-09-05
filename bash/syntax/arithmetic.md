# arithmetic

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
