# sed - stream editor

文本筛选和格式转换的流编辑器

`sed [OPTION]... {script-only-if-no-other-script} [input-file]...`

sed把当前处理行存储在临时缓冲区中，称为模式空间（pattern space），接着用sed命令处理缓冲区中的内容，处理完成后，把缓冲区的内容送往屏幕。接着处理下一行，这样不断重复，直到文件末尾，文件内容并没有改变。

```shell
# 删除文件中的空行
$ sed -i '/^$/d' test.txt

# 打印1-3行
$ sed -n '1,3p' test.txt

# 打印奇数行
$ sed -n '1~2p' test.txt

# 在第5行后加字符串
$ sed -i '5a hello' test.txt

# 最后一行添加
$ sed -i '$a hello' test.txt

# 在第3行前加字符串
$ sed -i '3i hello \
> world' test.txt

# 替换第1行
$ sed -i '1c hello' test.txt

# 查找并替换每行第一处匹配
$ sed 's/hello/Hello/' test.txt

# 查找并替换每行所有匹配
$ sed 's/hello/Hello/g' test.txt
```

## 地址

Sed命令可以没有指定地址，这样将会为所有行执行命令。如果给出了一个地址， 这样命令就只会为匹配此地址的行执行；如果给出了两个地址，这样命令将会对从匹配的第一个地址到第二个地址的区域的所有输入行执行命令。

### 地址类型

+ number 匹配特定的行
+ first~step 从first起，每隔step匹配
+ $ 匹配最后一行
+/regexp/ 匹配符合正则表达式regexp的行
+ **\c**regexp**c** 匹配符合正则表达式regexp的行，c可以是任意字符

`GNU sed` 还支持2地址格式

+ 0,addr2 从“匹配了第一个地址”状态始，直到发现addr2
+ addr1,+N addr1 及之后的 N 行
+ addr1,~N addr1 及之后的输入行数是 N 倍的下一行

## 命令

### 零地址命令

```
:           label b 和 t 命令的标签
#comment    注释将会延长到下一个新行 (或者一个 -e 脚本片段的结尾。）
}           一个{ }块的闭括号
```

### 零或一地址命令

```
=           打印当前行的行号

a \
text        追加 text, text可以通过预先写入反斜杠来嵌入新行

i \
text        插入 text, text可以通过预先写入反斜杠来嵌入新行

r filename  从文件名读取内容并追加到最后
R filename  从文件名读取一行并追加到最后,每次调用这个命令就从文件中读取一 行
```

### 接受范围地址的命令

```
{
        开始一个命令块,以 } 结束

b label
        程序跳转到label处;如果label被省略，跳转到脚本尾部

c \
text
        用 text 代替选择的行, text 可以通过预先写入反斜杠来嵌入新行

d
        删除模式空间。 开始下一个循环

D
        如果模式空间包含新行，在d命令发布之后开始一个新的普通循环。否则， 删除模式空间中的text直到新行，并且在不读入新行的情况下，用结果的 模式空间开始一个循环

h H
        复制/追加模式空间到保存空间

g G
        复制/追加保存空间到模式空间。

l
        用“直观明确”的格式列出当前行

l width
       用“直观明确”的格式列出当前行，在width字符处结束

n N
        读取/追加下一行输入到模式空间

p
        输出当前的模式空间

P
        输出直到遇到当前模式空间的第一个嵌入的换行符

s/regexp/replacement/
       尝试与模式空间匹配regexp，如果成功，用replacement代替匹配的部分

t label
    自上次输入行读取之后并且上次执行了t或者T命令之后，如果一个s///被 成功替换，那么跳转到label处;如果label被忽略，跳转到脚本结尾

T label
    自上次输入行读取之后并且上次执行了t或者T命令之后，如果没有s///被 成功替换，那么跳转到label处;如果label被忽略，跳转到脚本结尾

w filename
        将当前的模式空间写入到filename中

W filename
        写入当前模式空间的第一行到filename中

x
        交换模式空间和保存空间的内容

y/source/dest/
        将模式空间中显示为source的字符用dest字符来写出
```

## sed 选项

```
-n, --quiet, --silent
        不自动打印模式空间的内容

-e 脚本, --expression=脚本
        添加脚本到将要执行的命令

-f 脚本文件, --file=脚本文件
        添加脚本文件的内容到即将执行的命令

--follow-symlinks
        处理已存在文件时跟随链接

-i[SUFFIX], --in-place[=SUFFIX]
        编辑已存在的文件（如果提供了SUFFIX作为后缀则做备份）

-l N, --line-length=N
        指定'l'命令需要的自动换行长度

--posix
        禁用所有GNU插件。

-E, -r, --regexp-extended
        在脚本中使用扩展正则表达式（为了可移植性，请使用符合POSIX的-E）

-s, --separate
        将多个文件分别看待，而非统一视作同个连续的流。

--sandbox
        在沙箱模式下操作。

-u, --unbuffered
        从输入文件中读取最少量的数据并更频繁地刷新输出缓冲区

-z, --null-data
        用NUL字符分割行
```
