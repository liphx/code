```
do
end -- 相当于c中的代码段{}，有局部变量作用域
```


```
if .. then .. elseif .. then .. else .. end
while .. do .. end
repeat .. until .. 
for var = exp1, exp2, exp3 do .. end -- exp3是步长，不提供则为1，三个表达式都只计算一次，var是局部变量
```


```
goto name
::name::
```

```
模式匹配
.   所有字符
%a  字母
%c  控制字符
%d  数字
%g  除空格外的可打印字符
%l  小写字母
%p  标点字符
%s  空白字符
%u  大写字母
%w  字母数字字符
%x  十六进制数字
字母取大写则表示取反 %A 匹配除字母以外的字符

[] 里面是字符集 character class
[0-7] 01234567
[a-zA-Z] 字母 
[01] 二进制
[^\n] 除换行符
[%A] = [^%a]
+ 1个或多个
* 0个或多个 最长匹配
- 0个或多个 最短匹配，例如消除C语言注释'/%*.-%*/'
? 0或1

%a+ 一个单词
%d+ 一个整数
[+-]?%d+ 正负数，且符号可选

模式以^开头，匹配的子串是字符串的开头
模式以$结尾，匹配的子串是字符串的结尾

'%bxy' 匹配以x开头以y结尾的字符串
```

```
Precompiled Code
$ luac -o prog.lc prog.lua
$ lua prog.lc
```
