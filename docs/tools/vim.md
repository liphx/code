# VIM - Vi IMproved

## vim 命令行

* `vim [file ..]`

  * `vim`

        不带任何参数, 开始一个空白的缓冲区

  * `vim -`

        从标准输入读取文本

  * `vim file`

        编辑单个文件, 为避免以`-` 开头的文件被识别成命令行选项, 需在文件名前加 `--`, 例如文件名是`-txt`, 输入`vim -- -txt`

  * `vim file1 file2 ...`

        编辑多个文件, 读入缓冲区列表, 其中第一个文件会成为当前文件

* 定位

  * `vim file +[n]`

        光标定位在第 n 行, 如果不加 n 则定位在最后一行

  * `vim file +/{pat}`

        从光标位置(viminfo 中上次的位置)开始匹配 `pat` 并定位到这一行, `vim +5 +/abc` 从第5行开始匹配 abc

* 模式

  * `vim -e`

        以 Ex 模式启动

  * `vim -R`

        以只读模式启动

* 执行

  * `vim +{cmd}`
  * `vim -c {cmd}`

        读入第一个文件后执行 `cmd`

  * `vim --cmd {cmd}`

        在处理任何 `vimrc` 文件之前执行命令 `cmd`

  * `vim -u {vimrc}`

        读入文件 `vimrc` 执行初始化, `-u` 参数导致开启 `compatible`

  * `vim -u NONE`

        跳过所有文件和环境变量的初始化

  * `vim -u NORC`

        同 `NONE`,  但不跳过插件的载入

* 兼容

  * `vim -C`

        兼容模式

  * `vim -N`

        不兼容模式

## 模式

1. `Normal mode`

    也称`Command mode`, 进入 vim 的初始模式, 其他模式下输入 `ESC` 回到 `Normal`, `Ex` 模式下输入 `vi[sual]` 回到 `Normal`

2. `Visual mode`

    可视模式, `Normal` 下输入 `v`, `V` 或 `<C-V>` 进入

3. `Select mode`

    选择模式, `Visual` 下输入 `<C-G>` 进入, 选择模式下, 输入字符将删除高亮的文本并替换成新的字符, 进入 `Insert`

4. `Insert mode`

   插入模式, `Normal` 下输入 `i`, `I`, `a`, `A`, `o`, `O`, `s`, `S`  等进入

5. `Command-line mode`

    命令行模式, `Normal` 下输入 `:`, `/`, `?`, `!` 进入, 执行完 `Ex` 命令后回到 `Normal`

6. `Ex mode`

   类似 `Command-line`, 执行完 `Ex` 命令后还在 `Ex` 模式, `Normal` 下输入 `Q` 或 `gQ` 进入

## `Normal` 模式

### 移动与滚屏

| 输入         | 效果                              |
| ------------ | --------------------------------- |
| `h` `←`      | 向左移动                          |
| `j` `↓`      | 向下移动                          |
| `k` `↑`      | 向上移动                          |
| `l` `→`      | 向右移动                          |
| `gk` `gj`    | 屏幕行上下移动                    |
| `w`          | 移动到下一个单词开头              |
| `W`          | 移动到下一个字串开头              |
| `b`          | 反向移动一个单词至开头            |
| `B`          | 反向移动一个字串至开头            |
| `e`          | 移动到单词末尾                    |
| `E`          | 移动到字串末尾                    |
| `ge`         | 反向移动到单词末尾                |
| `gE`         | 反向移动到字串末尾                |
| `nw` `ne`    | 移动n(输入的数字)个单词           |
| `0` `<Home>` | 移动到行首                        |
| `$` `<End>`  | 移动到行尾                        |
| `g0`         | 移动到屏幕行行首                  |
| `^`          | 移动到行首第一个非空字符          |
| `g^`         | 移动到屏幕行行首第一个非空字符    |
| `g$`         | 移动到屏幕行行尾                  |
| `3$`         | 移动到下面第3行行尾，本行是第一行 |
| `3^`         | 依然作用于本行, 计数对 `^` 无效   |
| `<C-U>`      | 上移半屏                          |
| `<C-D>`      | 下移半屏                          |
| `<C-Y>`      | 上移一行                          |
| `<C-E>`      | 下移一行                          |
| `<C-B>`      | 上移一屏                          |
| `<C-F>`      | 下移一屏                          |
| `zz`         | 将当前行置于屏幕中央              |
| `zt`         | 将当前行置顶                      |
| `zb`         | 将当前行置底                      |

一些解释

* 实际行与屏幕行

    对于很长的一行文字, 屏幕上一行显示不全的情况, 一行实际行就对应多行屏幕行, `set number` 显示的行号就是实际行

* 单词与字串

  * 单词(`word`): 以非字母分隔(一串没有标点的中文为一个单词)
  * 字串(`WORD`): 以空白符分隔

* 移动行与滚屏

    移动行就是在行之间移动, 如无必要, 不会移动屏幕, 例如当前屏幕下显示 `20 - 60` 行, 从第 25 行移动到 31 行并不会移动屏幕,
    但移动到第 61 行会导致屏幕下移一行; 移屏同理, 必要时才会导致行的移动

### 插入文本

| 输入      | 效果                               |
| --------- | ---------------------------------- |
| `i`       | 在光标处插入文本                   |
| `ni`      | 在光标处插入字符n次                |
| `I`       | 在行首插入文本, 即 `^i`            |
| `a`       | 在光标后插入文本                   |
| `na`      | 在光标后插入字符n次                |
| `A`       | 在行尾添加文本, 即 `$a`            |
| `o`       | 在光标下行插入一行并进入插入模式   |
| `O`       | 在光标上行插入一行并进入插入模式   |
| `r`       | 在光标处替换字符                   |
| `R`       | 在光标处连续替换字符               |
| `c<CR>`   | 删除当前行并(在下一行)进入插入模式 |
| `S` `^c`  | 删除当前行并进入插入模式           |
| `cl` `s`  | 删除一个字符并进入插入模式         |
| `ce` `cw` | 删除一个单词并进入插入模式         |
| `c$` `C`  | 删除至行尾并进入插入模式           |

### 文本操作

| 输入      | 效果                                   |
| --------- | -------------------------------------- |
| `x`       | 删除光标所在位置的字符                 |
| `dw`      | 从当前光标当前位置直到下一个单词起始处 |
| `de`      | 从当前光标当前位置直到单词末尾         |
| `d$` `D`  | 从当前光标删除到行末                   |
| `dnw`     | 删除n个单词                            |
| `dd`      | 删除整行                               |
| `ndd`     | 删除n行                                |
| `daw`     | 删除光标所在的单词及其后的空格         |
| `J`       | 删除两行之间的换行符                   |
| `y`       | 在可视状态下输入y进行复制              |
| `yw` `yy` | 复制单词/整行                          |
| `p`       | 在光标后粘贴                           |
| `P`       | 在光标前粘贴                           |
| `gp` `gP` | 同 `p` `P`, 但光标停留在新文本之后     |

一些解释

1. vim 术语: `delete`, `yank` 与 `put`

### 文本对象

#### 操作分隔符

| 文本对象 | 选择区域                   |
| -------- | -------------------------- |
| a) 或 ab | 一对圆括号(parentheses)    |
| i) 或 ib | 圆括号(parentheses)内部    |
| a} 或 aB | 一对花括号{braces}         |
| i} 或 iB | 花括号{braces}内部         |
| a]       | 一对方括号[brackets]       |
| i]       | 方括号[brackets]内部       |
| a>       | 一对尖括号                 |
| i>       | 尖括号内部                 |
| a'       | 一对单引号'single quotes'  |
| i'       | 单引号'single quotes'内部  |
| a"       | 一对双引号"double quotes"  |
| i"       | 双引号"double quotes"内部  |
| a\`      | 一对反引号                 |
| i\`      | 反引号内部                 |
| at       | 一对XML标签<xml>tags</xml> |
| it       | XML标签内部                |

#### 操作文本块

| 文本对象 | 选择范围           |
| -------- | ------------------ |
| iw       | 当前单词           |
| aw       | 当前单词及一个空格 |
| iW       | 当前字串           |
| aW       | 当前字串及一个空格 |
| is       | 当前句子           |
| as       | 当前句子及一个空格 |
| ip       | 当前段落           |
| ap       | 当前段落及一个空行 |

### 文本可视

| 输入    | 效果                                                            |
| ------- | --------------------------------------------------------------- |
| `viw`   | 可视一个单词                                                    |
| `v`     | 面向字符的可视模式                                              |
| `V`     | 面向行的可视模式                                                |
| `<C-v>` | 面向列块的可视模式                                              |
| `gv`    | 重选上次的高亮选区                                              |
| `vi}`   | 高亮选中{}内部的文本                                            |
| `u`     | 撤消最后执行的命令                                              |
| `U`     | 撤消对整行的修改, `U` 本身是一个修改动作，因此用 `u` 来撤销 `U` |
| `<C-R>` | 撤销之前的撤销,取消一个 `u`                                     |

### 定位

| 输入            | 效果                                                                       |
| --------------- | -------------------------------------------------------------------------- |
| `<C-g>`         | 显示当前编辑的文件名和文件中光标位置                                       |
| `G`             | 跳到文件最后一行                                                           |
| `gg`            | 跳到文件第一行                                                             |
| `nG`            | 跳到 n 行                                                                  |
| `50%`           | 移动到中间的位置                                                           |
| `90%`           | 移动到90%的位置                                                            |
| `<C-o>`         | 回到之前的位置                                                             |
| `<C-i>` `<Tab>` | 回到新的位置                                                               |
| `m{a-zA-Z}`     | 设置标记位，小写位置标记只在每个缓冲区里局部可见，而大写位置标记则全局可见 |
| `'{mark}`       | 命令跳到位置标记所在行，并把光标置于该行第一个非空白字符上                 |
| \`{mark}        | 命令把光标移动到设置此位置标记时光标所在之处，它同时恢复行、列的位置       |
| `%`             | `()[]{}` 间跳转                                                            |
| `(`             | 跳转到上一句开头                                                           |
| `)`             | 跳转到下一句开头                                                           |
| `{`             | 跳转到上一段开头                                                           |
| `}`             | 跳转到下一段开头                                                           |
| `H/M/L`         | 跳到屏幕最上方/正中间/最下方                                               |
| `gf`            | 跳转到光标下的文件名, `:set suffixesadd+=` 可以加上扩展名                  |
| `<C-]>`         | 跳转到光标下关键字的定义之处                                               |

自动位置标记

| 位置标记 | 跳转到                           |
| -------- | -------------------------------- |
| \`\`     | 当前文件中上次跳转动作之前的位置 |
| ''       | 同\`\`                           |
| \`.      | 上次修改的地方                   |
| \`^      | 上次插入的地方                   |
| \`[      | 上次修改或复制的起始位置         |
| \`]      | 上次修改或复制的结束位置         |
| \`<      | 上次高亮选区的起始位置           |
| \`>      | 上次高亮选区的结束位置           |

### 缩进

| 输入      | 效果                |
| --------- | ------------------- |
| `>>` `>0` | 当前行缩进          |
| `>n`      | 当前行及以下n行缩进 |
| `>G`      | 当前行至文档尾缩进  |
| `<`       | 减少缩进            |
| `=`       | 自动缩进            |
| `gg=G`    | 自动缩进整个文件    |

### 其他命令

| 输入      | 效果                             |
| --------- | -------------------------------- |
| `ZZ`      | 保存后退出                       |
| `ga`      | 显示字符编码                     |
| `q:` `q/` | 命令行窗口                       |
| `@:`      | 重复上次的 Ex 命令               |
| `<C-^>`   | 切换当前文件与轮换文件           |
| `.`       | 重复上一次命令, 移动命令不会重复 |
| n`<C-a>`  | 对数字执行 +n 操作               |
| n`<C-x>`  | 对数字执行 -n 操作               |
| `g~`      | 切换大小写                       |
| `gu`      | 切换成小写                       |
| `gU`      | 切换成大写                       |

## `Visual` 模式

| 输入 | 效果                 |
| ---- | -------------------- |
| `o`  | 切换高亮选区的活动端 |

## Ex 命令

`Normal` 下输入 `:` 或 `Q` 进入相应的模式, 可以执行 `Ex` 命令(输入完命令后回车执行)

输入 `ESC` 或 `<C-[>` 可以回到正常模式或者撤消一个不想输入或部分完整的命令

| 命令                                            | 效果                                                                        |
| ----------------------------------------------- | --------------------------------------------------------------------------- |
| `[range]delete [x]`                             | 删除指定范围内的行[到寄存器x中]                                             |
| `[range]yank [x]`                               | 复制指定范围的行[到寄存器x中]                                               |
| `[line]put [x]`                                 | 在指定行后粘贴寄存器x中的内容                                               |
| `[range]copy {address}`                         | 把指定范围内的行拷贝到{address} 所指定的行之下, 不使用寄存器 简写为 :t, :co |
| `[range]move {address}`                         | 把指定范围内的行移动到{address}所指定的行之下 简写为 :m                     |
| `[range]join`                                   | 连接指定范围内的行                                                          |
| `[range]normal {commands}`                      | 对指定范围内的每一行执行普通模式命令{commands}                              |
| `[range]substitute/{pattern}/ {string}/[flags]` | 把指定范围内出现{pattern}的地方替换为{string}                               |
| `[range]global/{pattern}/[cmd]`                 | 对指定范围内匹配{pattern}的所有行，在其上执行Ex 命令{cmd}                   |
| `[range]print[flags]`                           | 显示 [range] 范围里的行 (缺省是当前行) 简写为 :p                            |
| `<C-f>`                                         | 切换到命令行窗口                                                            |
| `!{cmd}`                                        | 执行 shell 命令, 命令行中 `%` 代表当前文件名                                |
| `read !{cmd}`                                   | 把{cmd}命令的输出读入当前缓冲区中, 缓冲区即文件在内存中的映像               |
| `write !{cmd}`                                  | 把缓冲区内容作为指定{cmd}的标准输入                                         |
| `pwd`                                           | 打印当前目录                                                                |
| `reg "{reg}`                                    | 查看寄存器内容                                                              |

一些解释

* `range: {start,end}`
  * `0` 虚拟行，位于文件第一行上方
  * `1` 文件的第一行
  * `.` 当前行
  * `$` 最后一行
  * `%` 所有行
  * `start`, `end` 可以是数字也可以是模式
  * `{address}+n` 地址偏移
  * `'m` 包含位置标记m的行
  * `'<` 高亮选区的起始行
  * `'>` 高亮选区的结束行

### 缓冲区操作

| 输入                              | 效果                                                  |
| --------------------------------- | ----------------------------------------------------- |
| `w[rite]`                         | 把缓冲区内容写入磁盘                                  |
| `w file`                          | 保存在文件 file 中                                    |
| `q[uit]!`                         | 丢弃修改退出                                          |
| `e[dit]!`                         | 把磁盘文件内容读入缓冲区(即回滚所做修改)              |
| `qa[ll]!`                         | 关闭所有窗口，摒弃修改而无需警告                      |
| `wa[ll]!`                         | 把所有改变的缓冲区写入磁盘                            |
| `wq`                              | 保存后退出                                            |
| `ls`                              | 查看缓冲区列表                                        |
| `bnext`                           | 切换到列表中的下一个缓冲区                            |
| `bnext!`                          | 强制(未保存)切换缓冲区                                |
| `bprev`                           | 切换到列表中的上一个缓冲区                            |
| `bfirst`                          | 切换到列表中的第一个缓冲区                            |
| `blast`                           | 切换到列表中的最后一个缓冲区                          |
| `buffer N` `:buffer {bufname}`    | 直接跳转到一个缓冲区                                  |
| `bdelete N1 N2 N3` `:N,M bdelete` | 删除缓冲区                                            |
| `args`                            | 查看参数列表, [] 指明了参数列表中的哪个文件是活动文件 |
| `args {arglist}`                  | 设置参数列表                                          |
| `argdo`                           | 在列表中的每个缓冲区上执行同一条命令                  |
| `e[dit] file`                     | 通过文件的绝对路径或相对路径来打开文件                |
| `find`                            | 查找打开文件                                          |
| `set path+=`                      | 设置 find 路径                                        |
| `r file`                          | 将file文件的内容插入进来                              |
| `changes`                         | 查看改变列表                                          |
| `jumps`                           | 查看跳转列表                                          |
| `marks`                           | 查看标记列表                                          |

`ls`  的输出中

* `%` 指明哪个缓冲区在当前窗口中可见
* `#` 代表轮换文件
* `+` 表示这个缓冲区被修改过了
* `a` 表示活动缓冲区
* `h` 表示隐藏缓冲区

## `Insert` 模式

| 输入                                                                    | 效果                                                     |
| ----------------------------------------------------------------------- | -------------------------------------------------------- |
| `ESC` `<C-[>`                                                           | 回到正常模式                                             |
| `<C-o>`                                                                 | 插入-普通模式: 执行一次普遍模式命令后回到插入模式        |
| `<C-r>=`                                                                | 把执行的结果插入到文档的当前位置                         |
| `<C-v>{code}`                                                           | 用字符编码插入非常用字符                                 |
| 退格键 `<C-h>`                                                          | 删除前一个字符                                           |
| `<C-w>`                                                                 | 删除前一个单词                                           |
| `<C-u>`                                                                 | 删至行首                                                 |
| `<C-r>{register}`                                                       | 插入模式下粘贴寄存器中的文本, + 寄存器保存剪切板中的内容 |
| `<C-r><C-p>{register}` 按原义插入寄存器内的文本，并修正任何不必要的缩进 |

## 寄存器

| 寄存器        | 说明                                                                                    |
| ------------- | --------------------------------------------------------------------------------------- |
| `""`          | 无名寄存器, ""p 等同 p                                                                  |
| `"{register}` | 有名寄存器 `[a–zA-Z]`, 使用小写有名寄存器会覆盖寄存器的内容，大写有名寄存器则添加到后面 |
| `"0`          | 复制专用寄存器                                                                          |
| `"_`          | 黑洞寄存器                                                                              |
| `"+`          | X11剪贴板，用剪切、复制与粘贴命令操作                                                   |
| `"*`          | X11主剪贴板, 用鼠标中键操作                                                             |
| `"=`          | 表达式寄存器                                                                            |

| 只读寄存器          | 内容             |
| ------------------- | ---------------- |
| "%                  | 当前文件名       |
| "#                  | 轮换文件名       |
| ".                  | 上次插入的文本   |
| ":                  | 上次执行的Ex命令 |
| "/ (可用 :let 修改) | 上次查找的模式   |

## 搜索

`/string + 回车` 向下查找 要查找特殊字符需要用'\'来转义 `.*[]^%/\?~$`  
`?string + 回车` 逆向查找  
`n` 查找下一个  
`3n` 查找下面第3个  
`N` 查找上一个  
`/<CR>` 正向跳转至相同模式的下一处匹配  
`?<CR>` 反向跳转至相同模式的上一处匹配  
`%` 匹配括号，在`([{}])` 处按`%`会跳到匹配的括号处(再按一次就会回到之前的括号处)  
`f{char}` 到右侧第 [count] 次出现的字符 {char}。光标放在 {char}上  
`t{char}` 同f。光标放在 {char}左侧  
`F{char}` 同f。在左侧匹配  
`T{char}` 同f。在左侧匹配,光标放在{char}右侧  
`;` 重复上次的 f、t、F 或者 T 命令 [count] 次  
`,` 反方向重复上次的 f、t、F 或者 T 命令 [count] 次  
`f F t T ; ,` 只作用于本行  
`*` 正向查找第 [count] 次出现距离当前光标最近的单词的地方  
`#` 同 `*`，但反向查找  
`:nohlsearch` 暂时关闭查找高亮功能, 直到执行新的或重复的查找命令为止  
`<C-r><C-w>` 根据预览结果对查找域自动补全  
`//e<CR>` 查找偏移  
`\>` 标记单词结尾，`\<标记开头`  
`* #` 默认使用了`\<` `\>` 使用`g*` `g#` 来部分匹配

## 替换

`:[range]s[ubstitute]/{pattern}/{string}/[flags]`

`:s/old/new` 改变光标所在行的第一个匹配串  
`:s/old/new/g` 替换全行的匹配串  
`:%s/old/new/g` 替换整个文件中的每个匹配串  
`%s/old/new/gc` 找到整个文件中的每个匹配串，并且对每个匹配串提示是否进行替换  
`:#,#s/old/new/g` 替换两行之间出现的每个匹配串，#,# 代表的是替换操作的若干行中首尾两行的行号  
`:%s///gn` 统计当前匹配模式的个数, n 抑制正常的替换行为  
标志位e专门用于屏蔽这些错误提示  
标志位&仅仅用于指示Vim重用上一次substitute命令所用过的标志位  
`:&&` 命令重新执行替换操作, 本身只作用于当前行  
`:'<,'>&&`  作用于高亮选区
`:%&&` `g&` 作用于整个文件  

替换域中的特殊字符

| 符号 | 描述                                                               |
| ---- | ------------------------------------------------------------------ |
| `\r` | 插入一个换行符                                                     |
| `\t` | 插入一个制表符                                                     |
| `\\` | 插入一个反斜杠                                                     |
| `\1` | 插入第1个子匹配                                                    |
| `\2` | 插入第2个子匹配（以此类推，最多到\9）                              |
| `\0` | 插入匹配模式的所有内容                                             |
| `&`  | 插入匹配模式的所有内容                                             |
| `~`  | 使用上一次调用:substitute时的{string}                              |
| `\=` | {Vim script}执行{Vim Script}表达式；并将返回的结果作为替换{string} |

## global 命令

`:[range] global[!] /{pattern}/ [cmd]`

在缺省情况下，:global命令的作用范围是整个文件  
[cmd] 可以是除:global命令之外的任何Ex命, 缺省使用:print
:global! :vglobal（v表示invert）反转  
`:g/re/d` 删除所有行  
`:v/re/d` 只保留匹配行  

## 设置类命令

`:set`  
`ic` `ignorecase` 查找时忽略字母大小写  
`is` `incsearch` 查找短语时显示部分匹配  
`hls` `hlsearch` 高亮显示所有的匹配短语  
`/string\c` 在一次查找中忽略大小写  
在选项前加上 no 可以关闭选项

| set              | 作用     |
| ---------------- | -------- |
| set ignorecase   | 打开功能 |
| set noignorecase | 关闭     |
| set ignorecase!  | 反转     |
| set ignorecase?  | 查询     |
| set ignorecase&  | 设为默认 |
| set tabstop=2    | 设置值   |

`:edit $MYVIMRC` 编辑 vimrc  
`:source {file}` 加载配置  
`:source $MYVIMRC` 如果vimrc文件恰好是当前活动的缓冲区，则可把此命令简化为 :so %

## 帮助

`:help` `F1` 显示一般性的帮助信息  
`:help cmd` 显示cmd的帮助信息  
`CTRL W` 在窗口间跳转

## 配置

`~/.vimrc`文件

## 补全

`:set nocp` 不与Vi兼容模式  
`<TAB>` 尝试自动补全  
`CTRL D` 显示命令列表

## 自定义

`:map-operator` 自定义操作符  
`omap-info` 自定义动作命令

## 修改 root 用户文件

`:w !sudo tee % > /dev/null`

## 宏

录制宏

`q{register}` 开始录制  
`q` 停止  
`@{register}` 执行指定寄存器的内容  
`@@` 重复最近调用过的宏  
如果宏执行动作命令失败了，Vim将中止执行宏的其余命令  
`qa` 会替换之前创建的宏，`qA` 则追加到之前的宏

## 模式

设置查找时大小写不敏感 `:set ignorecase`, 同时也会影响vim关键字的自动补全  
查找时使用元字符 \c \C 可使这次查找忽略/不或略大小写  
`:set smartcase` 在全小写是忽略大小写，在含有大写时不忽略  
`\v` 开启very magic搜索模式，假定除 `_` 大小写字母以及数字0到9之外的所有字符都具有特殊含义  
`\V` 开启very nomagic搜索模式，即按原义查找文本
圆括号用以捕获，\1 \2 .. \9 表示第n个捕获项，\0 匹配整个串  
`\_s` 匹配空白符或换行符  
very magic搜索模式下，用  <与>符号表示单词定界符  
\w匹配单词类字符，包括字母、数字以及符号 `_`，而\W则用来匹配除单词类字符以外的其他字符  
在圆括号前面加上%，指 示Vim不要将括号内的内容赋给寄存器  
元字符\zs标志着一个匹配的起始，而元字符\ze则用来界定匹配的结束，例如
`/\v"\zs[^"]+\ze"<CR>` 模式包含字符串，但只匹配引号内的文本  

## 工具

### :grep :vimgrep

`:grep` vim 调用外部 grep，会创建 quickfix 列表  

`:vim[grep][!] /{pattern}/[g][j] {file} ...`

### 自动补全

开启 ignorecase 后，自动补全也会忽略大小写  
`:set infercase` 仍会联想大小写，补全时不会替换已输入字符的大小写  

触发Vim自动补全的方法

| 命令               | 补全类型         |
| ------------------ | ---------------- |
| `<C-n>` 或 `<C-p>` | 普通关键字       |
| `<C-x><C-n>`       | 当前缓冲区关键字 |
| `<C-x><C-i>`       | 包含文件关键字   |
| `<C-x><C-]>`       | 标签文件关键字   |
| `<C-x><C-k>`       | 字典查找         |
| `<C-x><C-l>`       | 整行补全         |
| `<C-x><C-f>`       | 文件名补全       |
| `<C-x><C-o>`       | 全能（Omni）补全 |

补全菜单交互命令

| 按键操作            | 作用                                                           |
| ------------------- | -------------------------------------------------------------- |
| `<C-n>`             | 使用来自补全列表的下一个匹配项（next匹配项）                   |
| `<C-p>`             | 使用来自补全列表的上一个匹配项（previous匹配项）               |
| `<Down>`            | 选择来自补全列表的下一个匹配项                                 |
| `<Up>`              | 选择来自补全列表的上一个匹配项                                 |
| `<C-y>`             | 确认使用当前选中的匹配项（yes）                                |
| `<C-e>`             | 还原最早输入的文本（从自动补全中exit）                         |
| `<C-h>`（与`<BS>`） | 从当前匹配项中删除一个字符                                     |
| `<C-l>`             | 从当前匹配项中增加一个字符{char} 中止自动补全并插入字符{char}@ |

### 拼写检查

`:set spell` 开启拼写检查  
`set spelllang=en_us` 设置语言  

| 命令  | 用途                         |
| ----- | ---------------------------- |
| `]s`  | 跳到下一处拼写错误           |
| `[s`  | 跳到上一处拼写错误           |
| `z=`  | 为当前单词提供更正建议       |
| `zg`  | 把当前单词添加到拼写文件中   |
| `zw`  | 把当前单词从拼写文件中删除   |
| `zug` | 撤销针对当前单词的zg或zw命令 |

`spellfile` 指定文件保存由zg 和zw 命令所添加、删除的单词

插入模式下通过`<C-x>s` `<C-x><C-s>`更正拼写错误，该命令会触发一个特殊的自动补全功能

开启 expandtab `<C-v><Tab>` 会插入制表符

### 自动命令

`autocmd {event} {pattern} {cmd}`

在 `cmd` 中，`<afile>` 代表当前文件名

多个 `pattern` 以逗号分隔

`FileType` 事件用于匹配文件类型，`set filetype?` 查看文件类型

销毁自动命令 `autocmd!`

自动命令组 `augroup`

### bash 绑定 vi 键位

`set -o emacs` `set -o vi` 分别设置交互式 bash 的键位为 emacs  和 vi 模式, 默认是 emacs 模式

vi 模式下，默认是插入模式，按 ESC 回到一般模式

# ctags

## 安装与使用

* <http://ctags.sourceforge.net/>
* <https://github.com/universal-ctags/ctags>

```shell
sudo apt install exuberant-ctags universal-ctags
ctags files
```

## vim 跳转

| 命令               | 用途                                                                                                             |
| ------------------ | ---------------------------------------------------------------------------------------------------------------- |
| `<C-]>`            | 跳转到匹配当前光标所在关键字的第一处标签                                                                         |
| `g<C-]>`           | 如果有多处标签可以匹配当前光标所在的关键字，提示用户指定一处进行跳转。如果只有一处匹配，则不会提示，直接进行跳转 |
| `:tag {keyword}`   | 跳转到匹配{keyword}的第一处标签                                                                                  |
| `:tjump {keyword}` | 提示用户从匹配{keyword}的多处标签中指定一处进行跳转。如果只有一处匹配，则不会提示，直接进行跳转                  |
| `:pop` 或 `<C-t>`  | 反向遍历标签历史                                                                                                 |
| `:tag`             | 正向遍历标签历史                                                                                                 |
| `:tnext`           | 跳转到下一处匹配的标签                                                                                           |
| `:tprev`           | 跳转到上一处匹配的标签                                                                                           |
| `:tfirst`          | 跳转到第一处匹配的标签                                                                                           |
| `:tlast`           | 跳转到最后一处匹配的标签                                                                                         |
| `:tselect`         | 提示用户从标签匹配列表中选择一项进行跳转                                                                         |

## 启动vim时跳转

```shell
vim -t tag
```

需要有 `tags` 文件, 定位到标签函数(`C/C++`)的开始处

## 制作 stdc++ tags

```shell
mkdir ~/.vim/tags
cd ~/.vim/tags/
ctags -R --c++-kinds=+l+x+p --fields=+iaSl --extra=+q --language-force=c++ -f stdcpp.tags /usr/include/c++/10/
```

```vim
:set tags+=~/.vim/tags/stdcpp.tags
```

# 会话

## 保存会话

`:mksession!` 将当前会话写入 `Session.vim` 中

## 载入会话

shell中使用 `vim -S { file }` 读入文件后执行 `file`, 如果不带 `file` 参数等价于 `vim -S Session.vim`

或 vim 中执行 `:source Session.vim`

`:mksession` 与 `vim -S` 默认在当前目录保存和寻找文件，也可以指定文件名来保存和寻找，或是建立多个会话

## 持久化撤销

`set undofile` 开启持久化撤销

`set undodir=` 设置文件存储撤销历史

## 插件 obsession

```shell
git clone https://github.com/tpope/vim-obsession.git ~/.vim/pack/bundle/start/obsession
```

* `:Obsession` 开启/暂停自动保存会话
* `:Obsession!` 删除`Session.vim` 并停止自动保存会话

# 窗口与标签页

## 启动vim时分割

* `vim -o[N]`

    打开 `N` 个窗口，水平分割。如果没有给出 `N`，为每个文件参数打开一个窗口。
    如果没有足够的空间，只打开前面数项。如果给出的窗口数 超过参数的数目，后面的窗口会编辑空文件。

* `vim -O[N]`

    同 `-o`, 但垂直分割

* `vim -p[N]`

    同 `-o`, 标签页分割, 最大标签页数由 `tabpagemax` 决定

## 窗口

| 指令               | 作用                                     |
| ------------------ | ---------------------------------------- |
| `<C-w>s`           | 水平切分窗口                             |
| `<C-w>v`           | 垂直切分窗口                             |
| `:edit`            | 把另外一个缓冲区载入活动窗口中           |
| `:sp[lit] {file}`  | 水平切分当前窗口，并在新窗口中载入{file} |
| `:vsp[lit] {file}` | 垂直切分当前窗口，并在新窗口中载入{file} |

### 窗口间切换

| 指令     | 作用                          |
| -------- | ----------------------------- |
| `<C-w>w` | `<C-w><C-w>` 在窗口间循环切换 |
| `<C-w>h` | 切换到左边的窗口              |
| `<C-w>j` | 切换到下边的窗口              |
| `<C-w>k` | 切换到上边的窗口              |
| `<C-w>l` | 切换到右边的窗口              |
| `<C-w>r` | 交换两个窗口                  |

### 关闭窗口

| Ex 命令  | 普通模式命令 | 用途                             |
| -------- | ------------ | -------------------------------- |
| :clo[se] | <C-w>c       | 关闭活动窗口                     |
| :on[ly]  | <C-w>o       | 只保留活动窗口，关闭其他所有窗口 |

### 改变窗口大小及重新排列窗口

| 指令       | 作用                         |
| ---------- | ---------------------------- |
| `<C-w>=`   | 使所有窗口等宽、等高         |
| `<C-w>_`   | 最大化活动窗口的高度         |
| `<C-w>     | `                            | 最大化活动窗口的宽度      |
| `[N]<C-w>` | _  把活动窗口的高度设为[N]行 |
| `[N]<C-w>  | `                            | 把活动窗口的宽度设为[N]列 |
| `<C-w>+`   | 扩大窗口行数                 |
| `<C-w>-`   | 减少窗口行数                 |
| `<C-w>>`   | 扩大窗口列数                 |
| `<C-w><`   | 减少窗口列数                 |

## 标签页

| 指令                 | 作用                                   |
| -------------------- | -------------------------------------- |
| `:tabedit {file}`    | 新标签页编辑文件                       |
| `:tabnew`            | 新标签页编辑新文件                     |
| `:tabs`              | 列出标签页                             |
| `:tabclose`          | 关闭当前标签页                         |
| `:tabonly`           | 仅保留当前标签页                       |
| `:tabn`              | `:tabnext` `gt` 移动到下一个标签页     |
| `:tabp`              | `:tabprevious` `gT` 移动到上一个标签页 |
| `:tabm :tabmove [N]` | 把当前标签页移到第 N 个标签页之        |

# vimdiff

`vimdiff file1 file2` 竖屏比较两个文件  
`vimdiff -o file1 file2` 横屏比较两个文件  

vimdiff 同 `vim -d`

vim 编辑文件时 `:vertical diffsplit file2` 亦可比较

两侧的屏幕滚动默认同步，`:set noscrollbind` 设置不同步

| 指令                     | 作用                             |
| ------------------------ | -------------------------------- |
| `]c`                     | 下一个差异点                     |
| `[c`                     | 上一个差异点                     |
| `dp`                     | (diff put) 从当前复制到另一个    |
| `do`                     | (diff obtain) 从另一个复制到当前 |
| `:diffupdate`            | 刷新比较结果                     |
| `:set diffopt=context:6` | 缺省的上下文行数                 |
| `zo`                     | 打开折叠                         |
| `zc`                     | 重新折                           |

# vim 终端

`:ter[minal]` 打开终端

# quickfix

`:make` 在 vim 中编译  
`:make!` 出错后光标位置不变  

Quickfix 命令列表

| 命令      | 用途                         |
| --------- | ---------------------------- |
| `:cnext`  | 跳转到下一项                 |
| `:cprev`  | 跳转到上一项                 |
| `:cfirst` | 跳转到第一项                 |
| `:clast`  | 跳转到最后一项               |
| `:cnfile` | 跳转到下一个文件中的第一项   |
| `:cpfile` | 跳转到上一个文件中的最后一项 |
| `:cc N`   | 跳转到第 N 项                |
| `:copen`  | 打开quickfix窗口             |
| `:cclose` | 关闭quickfix窗口             |

## 定制外部编译器

`:set makeprg=` 可用 % # 扩展成当前文件名与轮换文件名

例如，对于markdown 文件, 设置

```vim
setlocal makeprg=pandoc\ -c\ common.css\ -s\ %\ -o\ ~/www/preview.html
```

即可通过 `:make` 生成预览文件

# vim script

## `hello.vim`

```vim
function! SayHello()
    echo 'hello, world'
endfunction

command! Hello call SayHello()
nnoremap Q :Hello<CR>
```

vim 中，使用 `:source {path}` 加载vim script  
`Q` 和 `:Hello` 均会输出 "hello, world"

## 注释

```vim
" 单行注释
```

## 变量

### 作用域

let 声明一个变量，变量存在三种作用域，全局、局部、脚本变量，可以指定前缀区别作用域

| 前缀 | 说明                                 |
| ---- | ------------------------------------ |
| `g:` | 全局变量                             |
| `l:` | 局部变量，只可在函数内部使用         |
| `s:` | 脚本变量，只可以在当前脚本函数内使用 |
| `v:` | Vim 特殊变量                         |
| `b:` | 作用域限定在某一个缓冲区内           |
| `w:` | 作用域限定在窗口内部                 |
| `t:` | 作用域限定在标签内部                 |

例如 `let g:str = "hello"` 定义一个全局的字符串变量

不指定前缀则为默认的作用域，函数外部为全局的，函数内部为局部的

### 变量类型

| 类型       | ID  | 描述     |
| ---------- | --- | -------- |
| Number     | 0   | 整数     |
| String     | 1   | 字符串   |
| Funcref    | 2   | 函数指针 |
| List       | 3   | 列表     |
| Dictionary | 4   | 字典     |
| Float      | 5   | 浮点数   |
| Boolean    | 6   |          |
| None       | 7   |          |
| Job        | 8   |          |
| Channel    | 9   |          |

## Vim 自定义命令

```vim
command! -nargs=* -complete=custom,helloworld#complete HelloWorld call helloworld#test()
```

紧接 command 命令其后的 ! 表示强制定义该命令，即使前面已经定义过了同样名称的命令，也将其覆盖掉

`-nargs=*` 表示，该命令可接受任意个数的参数， 包括 0 个。-nargs 的取值有以下几种情况

| 参数       | 定义                     |
| ---------- | ------------------------ |
| `-nargs=0` | 不接受任何参数（默认）   |
| `-nagrs=1` | 只接受一个参数           |
| `-nargs=*` | 可接收任意个数参数       |
| `-nargs=?` | 可接受 1 个或者 0 个参数 |
| `-nargs=+` | 至少提供一个参数         |

`-complete` 指定补全方式

# 插件(plugin), 包(package)

## plugin

插件的目录结构

```
demo-plugin/
├── doc
│   └── demo.txt
└── plugin
    └── demo.vim
```

安装插件

```vim
set runtimepath+=$VIMCONFIG/arbitrary/demo-plugin
```

## package

包的目录通常为 `$VIMCONFIG/pack`

vim 启动时会遍历 `$VIMCONFIG/pack/*/start` 目录下的插件

索引新安装插件的文档 `:helptags {path}` `:helptags ALL`

## 在包里安装插件

```shell
mkdir -p $VIMCONFIG/pack/bundle/{start,opt}
cd $VIMCONFIG/pack/bundle/start
git clone <git url>
vim -u NONE -c "helptags $VIMCONFIG/pack/bundle/start/<doc-path>" -c q
```

`$VIMCONFIG/pack/bundle/opt` 目录为可选插件，通过 `:packadd {name}` 激活插件 `pack/*/opt/{name}`

# Vim 插件

## vimcdoc

```shell
git clone https://github.com/yianwillis/vimcdoc.git ~/.vim/pack/bundle/start/vimcdoc
```

## NERDTree

```shell
git clone https://github.com/preservim/nerdtree.git ~/.vim/pack/bundle/start/nerdtree
vim -u NONE -c "helptags ~/.vim/pack/bundle/start/nerdtree/doc" -c q
```

| 命令        | 说明                         |
| ----------- | ---------------------------- |
| `:NERDTree` | 打开文档树                   |
| `q`         | 退出文档树                   |
| `o`         | 在之前的窗口打开             |
| `go`        | 预览, 打开但不跳转           |
| `t`         | 新标签页打开                 |
| `T`         | 新标签页打开但不跳转         |
| `i`         | 水平分割打开                 |
| `gi`        | 水平分割预览                 |
| `s`         | 垂直分割打开                 |
| `gs`        | 垂直分割预览                 |
| `I`         | 显示/关闭 隐藏文件           |
| `r`         | 刷新当前目录                 |
| `C`         | 把文档树根目录设为选择的目录 |
| `u`         | 文档树根目录上移一级         |
| `U`         | 同 `u` 但旧目录保持打开状态  |
| `cd`        | 设置 CWD 为选中的目录        |
| `CD`        | 设置文档树根目录为 CWD       |
| `?`         | help                         |

## YouCompleteMe

```shell
git clone https://github.com/ycm-core/YouCompleteMe.git ~/.vim/pack/bundle/start/YouCompleteMe
cd ~/.vim/pack/bundle/start/YouCompleteMe
git submodule update --init --recursive
python3 install.py --clangd-completer # or --all
```

## fzf

```shell
sudo apt install fzf
git clone https://github.com/junegunn/fzf ~/.vim/pack/bundle/start/fzf
```

| 命令           | 说明               |
| -------------- | ------------------ |
| `:FZF`         | 打开模糊匹配器     |
| `<C-c>`        | 关闭模糊匹配器     |
| `<C-j>` `down` | 选择匹配结果下一项 |
| `<C-k>` `up`   | 选择匹配结果上一项 |
| `<CR>`         | 打开当前项         |
| `<C-x>`        | 水平分割打开文件   |
| `<C-v>`        | 垂直分割打开文件   |
| `<C-t>`        | 新标签页打开       |

## dispatch

```shell
git clone https://github.com/tpope/vim-dispatch.git ~/.vim/pack/bundle/start/dispatch
```

`:Make` 前台运行构建程序  
`:Make!` 后台运行构建程序  
`:Make` `:Make!` 都是异步执行  
`:Dispatch` 切换编译器  

## commentary

```shell
git clone https://github.com/tpope/vim-commentary.git ~/.vim/pack/bundle/start/commentary
```

`:[range]Commentary` 快速注释/取消注释

## base16-vim

```shell
mkdir ~/.vim/colors
git clone https://github.com/chriskempson/base16-vim.git
cp base16-vim/colors/base16-*.vim ~/.vim/colors/
```

`:colorscheme {name}`  选择配色

## airline

```shell
git clone https://github.com/vim-airline/vim-airline.git ~/.vim/pack/bundle/start/airline
```

## surround

```shell
git clone https://github.com/tpope/vim-surround.git ~/.vim/pack/bundle/start/surround
```

* 把 `"` 改为 `'` 或 `<p>`: `cs"'` `cs"<p>`
* 删除 `"`: `ds"`
* 添加 `"`: `ysiw"`
* 整行添加`"`: `yss"`

## fugitive

```shell
git clone https://github.com/tpope/vim-fugitive.git ~/.vim/pack/bundle/start/fugitive
```

## ALE(Asynchronous Lint Engine)

```shell
git clone https://github.com/dense-analysis/ale.git ~/.vim/pack/bundle/start/ale
```

## grepper

```shell
git clone https://github.com/mhinz/vim-grepper.git ~/.vim/pack/bundle/start/grepper
```

## vim-test

```shell
git clone https://github.com/vim-test/vim-test.git ~/.vim/pack/bundle/start/vim-test
```

## goyo

```shell
git clone https://github.com/junegunn/goyo.vim.git ~/.vim/pack/bundle/start/goyo
```

`:Goyo` `:Goyo!` 切换状态

## vim-gutentags

```shell
git clone https://github.com/ludovicchabant/vim-gutentags.git ~/.vim/pack/bundle/start/vim-gutentags
```

## rainbow

```shell
git clone https://github.com/luochen1990/rainbow.git ~/.vim/pack/bundle/start/rainbow
```

```vim
let g:rainbow_active = 1    " 默认开启
:RainbowToggle      " 切换
:RainbowToggleOn    " 开启
:RainbowToggleOff   " 关闭
```

# neovim

## 安装与使用

```shell
sudo apt install neovim
nvim [options] [file ...]
```

## 配置

```shell
alias neovim=nvim
export NVIMCONFIG=~/.config/nvim
export NVIMDATA=~/.local/share/nvim
```

复用 vim 配置，`~/.config/nvim/init.vim`

```vim
set runtimepath^=~/.vim runtimepath+=~/.vim/after
let &packpath = &runtimepath
source ~/.vim/vimrc
```

## neovim provider

```shell
pip3 install --user --upgrade neovim
```

neovim 中 `:py3 {statement}` 启用 python3

## neovim-remote

```shell
pip3 install --user --upgrade neovim-remote
nvr --help
```

## neovim 终端模拟器

打开终端缓冲区 `:terminal`，终端缓冲区输入 `i` 进入终端模式，再按下 `<C-\><C-n>` 回到普通模式

创建终端缓冲区执行命令 `:terminal {cmd}`，简写为 `:te {cmd}`

新窗口打开终端缓冲区 `:[v]split | terminal`

新标签页打开 `:tabedit | terminal`

`jobsend()` 向终端缓冲区中正在运行的进程的标准输入写入

在当前 neovim 实例中打开文件

| 命令                        | 说明                               |
| --------------------------- | ---------------------------------- |
| `nvr <file>`                | 在当前窗口打开                     |
| `nvr -l <file>`             | 在最后激活的窗口打开               |
| `nvr -o <file> [<file>...]` | 通过 `:split` 打开一个或多个文件   |
| `nvr -O <file> [<file>...]` | 通过 `:vsplit` 打开一个或多个文件  |
| `nvr -p <file> [<file>...]` | 通过 `:tabedit` 打开一个或多个文件 |

重命名终端缓冲区 `:file {name}`

## 参考

### 网站

* <https://www.vim.org/>
* <https://yianwillis.github.io/vimcdoc/doc/help.html>
* <https://neovim.io/>

### 书籍

* [Vim实用技巧](https://book.douban.com/subject/25869486/)
* [精通 Vim](https://book.douban.com/subject/35005327/)
