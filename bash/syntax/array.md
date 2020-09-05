# 数组

## 普通数组

```shell
#!/bin/bash 
array_var=(test1 test2 test3 test4) #这些值将会存储在以0为起始索引的连续位置上
echo ${array_var[0]} # test1

array_var[0]=ttt
echo ${array_var[0]} # ttt

i=3
echo ${array_var[$i]} # test4

array_var[4]=test5
echo ${array_var[*]} # ttt test2 test3 test4 test5

array_var[10]=test6
echo ${array_var[*]} # ttt test2 test3 test4 test5 test6
echo ${array_var[@]} # ttt test2 test3 test4 test5 test6
echo ${#array_var[*]} # 6
echo ${!array_var[*]} # 0 1 2 3 4 10
```

## 关联数组

首先，需要使用声明语句将一个变量定义为关联数组

```shell
#!/bin/bash 
declare -A ass_array 

ass_array=([index1]=val1 [index2]=val2) 
echo ${ass_array[index1]} # val1

ass_array[index3]=val3
echo ${ass_array[index3]} # val3

echo ${#ass_array[*]} # 3
echo ${ass_array[*]} # val1 val3 val2
echo ${!ass_array[*]} # index1 index3 index2
```
