[toc]

# Python3

# 包管理程序 pip

## 使用 pip 安装 Python 模块

```shell
python3 -m pip install SomePackage
# or
pip3 install SomePackage

pip3 install SomePackage==1.0.1         # 指定版本号
pip3 install 'SomePackage>=1.0.1'       # 指定最低版本号

pip3 install --upgrade SomePackage      # 更新

pip3 uninstall [-y] SomePackage         # 卸载

pip3 install pip --upgrade              # 更新 pip
```

pip 其它选项

| 选项 | 说明 |
| -- | -- |
| `--trusted-host <host>`  | 信任的主机名 |
| `--retries <N>`   | 重试次数 |
| `--timeout <sec>` | 设置超时时间 |
| `--proxy <proxy>` | 指定代理 |
| `--no-cache-dir`  | 禁用缓存 |

`pip install` 其它选项

| 选项 | 说明 |
| -- | -- |
| `-i,--index-url <url>` | 指定 url, 默认为 <https://pypi.python.org/simple/> |
| `--no-deps` | 不安装依赖 |
| `-t,--target <dir>` | 安装至 dir |
| `-d,--download <dir>` | 仅下载至 dir，不安装 |
| `-r,--requirement <file>` | 安装在 file 中列出需要安装的包，file 包含由 `pip install` 参数排成列表的文件 |

### pip 配置文件

`unix`: `$HOME/.pip/pip.conf`

`win`: `%HOME%\pip\pip.ini`

```pip
[global]
timeout = 120
index-url = http://pypi.liph.ink/simple
trusted-host = liph.ink

[install]
find-links =
    http://mirror1.example.com
    http://mirror2.example.com
```

### pip 自动补全

```shll
pip3 completion --bash >> ~/.profile
```
## 安装使用 Python

### Linux下从源代码构建

1. 下载最新Python 源代码 <https://www.python.org/downloads/source/>
2. 构建

```shell
./configure
make
sudo make install
```

`make install` 会覆盖系统中已安装的Python 版本

查看configure 配置 `./configure --help`

| configure 选项 | 说明 |
| -- | -- |
| --prefix=PREFIX       | 指定PREFIX 安装架构无关的文件，默认为 `/usr/local` |
| --exec-prefix=EPREFIX | 指定PREFIX 安装依赖架构的文件 |

文件安装路径

| 文件/目录 | 说明 |
| -- | -- |
| `exec_prefix/bin/python3` | 解释器的推荐位置 |
| `prefix/lib/pythonversion`, ` exec_prefix/lib/pythonversion` | 包含标准模块的目录的推荐位置 |
| `prefix/include/pythonversion`, `exec_prefix/include/pythonversion` | 包含开发Python扩展和嵌入解释器所需的include文件的目录的推荐位置 |

查看 Python3 版本号 `python3 --version`

### Python 解释器

终端执行 `python3` 进入交互式 Python, 键入 Ctrl-D 退出交互式（windows 下是Ctrl-Z）, 或者输入 `sys.exit()` 退出（需要先 `import sys`）

python3 提供和bash 类似的 `-c` 选项以执行一条语句 `python3 -c command [arg] ...`

`python3 file` 用以解释执行 Python 源文件, Linux 下Python默认的字符编码是 UTF-8, 可以在源文件第一行指定编码

```py
# -*- coding: utf-8 -*-
```

如果第一行以 `shebang` 开头，则在第二行指定编码

```py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
```
## 作用域和名称空间

### global, nonlocal

```
global_stmt ::=  "global" identifier ("," identifier)*
nonlocal_stmt ::=  "nonlocal" identifier ("," identifier)*
```

#### global

```py
var = 'global var'

def func1():
    var = 'func1 var'

def func2():
    global var
    var = 'func2 var'


print(var) # global var
func1()
print(var) # global var
func2()
print(var) # func2 var
```

global 语句是作用于整个当前代码块的声明，用于给全局变量赋值

在 global 语句中列出的名称不得在同一代码块内该 global 语句之前的位置中使用；例如下面的代码语法是错误的

```py
a = 1
global a
```

#### nonlocal

```py
def func1():
    var = 'func1 var'
    def func2():
        var = 'func2 var'

    print(var) # func1 var
    func2()
    print(var) # func1 var

func1()

def func3():
    var = 'func3 var'
    def func4():
        nonlocal var
        var = 'func4 var'

    print(var) # func3 var
    func4()
    print(var) # func4 var

func3()
```

nonlocal 语句会使得所列出的名称指向之前在最近的包含作用域中绑定的除全局变量以外的变量

与 global 语句中列出的名称不同，nonlocal 语句中列出的名称必须指向之前存在于包含作用域之中的绑定（在这个应当用来创建新绑定的作用域不能被无歧义地确定）

nonlocal 语句中列出的名称不得与之前存在于局部作用域中的绑定相冲突

以下代码的语法是错误的

```py
var = 'global var'

def func():
    nonlocal var
    var = 'func var'
```
## 模块和包

### 模块

Python 模块即一个 Python 源文件(`.py`)

全局变量 `__name__` 定义了模块名，以 `python3 xxx.py <arguments>` 运行模块时，`__name__` 赋值为 `"__main__"`，因此可以添加如下代码让该模块仅当脚本运行时才执行这些语句

```py
if __name__ == "__main__":
    do some things
```

### import

```
import_stmt     ::=  "import" module ["as" identifier] ("," module ["as" identifier])*
                     | "from" relative_module "import" identifier ["as" identifier]
                     ("," identifier ["as" identifier])*
                     | "from" relative_module "import" "(" identifier ["as" identifier]
                     ("," identifier ["as" identifier])* [","] ")"
                     | "from" module "import" "*"
module          ::=  (identifier ".")* identifier
relative_module ::=  "."* module | "."+
```

import, import-from 语句用来导入其它模块，import 模块时会执行模块中的语句，导入模块中的符号

import 语句

1. 查找一个模块，如果有必要还会加载并初始化模块
2. 在局部命名空间中为 import 语句发生位置所处的作用域定义一个或多个名称

import-from 语句

1. 查找 from 子句中指定的模块，如有必要还会加载并初始化模块
2. 对于 import 子句中指定的每个标识符
  1. 检查被导入模块是否有该名称的属性
  2. 如果没有，尝试导入具有该名称的子模块，然后再次检查被导入模块是否有该属性
  3. 如果未找到该属性，则引发 `ImportError`
  4. 否则的话，将对该值的引用存入局部命名空间，如果有 as 子句则使用其指定的名称，否则使用该属性的名称

假如文件 `moudle.py` 定义了函数 `func`，那么，可以在其它文件中 `import moudle` 后，通过 `moudle.func()` 调用该模块中的函数，模块名后使用 as 时，直接把 as 后的名称与导入模块绑定

`from moudle import *` 可以导入模块内定义的所有名称

### 模块的搜索路径

搜索模块 `spam` 的顺序

1. 查找名为 `spam` 的内置模块
2. 从 `sys.path` 中查找文件 `spam.py`

`sys.path` 初始时包含

- 输入脚本的目录（或未指定文件时的当前目录）
- `PYTHONPATH` （目录列表，与 shell 变量 PATH 的语法一样）
- 默认安装目录

```py
>>> import sys
>>> sys.path
['', '/usr/local/lib/python39.zip', '/usr/local/lib/python3.9', '/usr/local/lib/python3.9/lib-dynload', '/home/liph/.local/lib/python3.9/site-packages', '/usr/local/lib/python3.9/site-packages']
```

### 包

包是一种组织模块的结构，可以用来构造Python 的名称空间

一个包含Python 模块的目录要称为包，需要在目录下编写一个 `__init__.py` 的文件，最简单的情况下，该文件为空；也可以执行包的初始化代码，或设置 `__all__` 变量

对于 `from package import item`

item 可以是包的子模块（或子包），也可以是包中定义的函数、类或变量等其他名称。import 语句首先测试包中是否定义了 item；如果未在包中定义，则假定 item 是模块，并尝试加载。如果找不到 item，则触发 ImportError 异常

对于 `from package import *`

- 如果包的 `__init__.py` 代码定义了列表 `__all__`，就导入列表中的模块
- 如果没有定义 `__all__`
  - 不会把包中所有子模块都导入到当前命名空间
  - 只确保导入包 package（可能还会运行 `__init__.py` 中的初始化代码）
  - 然后，再导入包中定义的名称。这些名称包括 `__init__.py` 中定义的任何名称（以及显式加载的子模块），还包括之前 import 语句显式加载的包里的子模块

在包中引入其它模块或子包中的模块，可用相对形式导入（基于当前模块名）

```py
from . import echo
from .. import formats
from ..filters import equalizer
```

由于，主模块名是 `"__main__"` ，因此，Python 应用程序的主模块必须始终使用绝对导入
## 函数

### 定义函数

```
funcdef                   ::=  [decorators] "def" funcname "(" [parameter_list] ")"
                               ["->" expression] ":" suite
decorators                ::=  decorator+
decorator                 ::=  "@" assignment_expression NEWLINE
dotted_name               ::=  identifier ("." identifier)*
parameter_list            ::=  defparameter ("," defparameter)* "," "/" ["," [parameter_list_no_posonly]]
                                 | parameter_list_no_posonly
parameter_list_no_posonly ::=  defparameter ("," defparameter)* ["," [parameter_list_starargs]]
                               | parameter_list_starargs
parameter_list_starargs   ::=  "*" [parameter] ("," defparameter)* ["," ["**" parameter [","]]]
                               | "**" parameter [","]
parameter                 ::=  identifier [":" expression]
defparameter              ::=  parameter ["=" expression]
funcname                  ::=  identifier
```

```py
def fib(n):
    """Print a Fibonacci series up to n."""
    a, b = 0, 1
    while a < n:
        print(a, end=' ')
        a, b = b, a+b
    print()

fib(2000)
```

* 函数定义使用 `def`，后跟函数名和参数列表
* 函数内的第一条语句是字符串时，称为文档字符串（docstring）
* 没有 `return` 或 `return` 不带表达式的函数返回 `None`

### 默认参数

```py
def func(a, b = 0):
    print(a > b)

func(1)
func(1, 2)
```

调用时需给出必选参数，可选参数按顺序给定

```py
i = 5

def f(arg=i):
    print(arg)

i = 6
f() #=> 5
```

默认值在定义作用域里的函数定义中求值

```py
def f(a, L=[]):
    L.append(a)
    return L

print(f(1)) # [1]
print(f(2)) # [1, 2]
print(f(3)) # [1, 2, 3]
```

默认值只计算一次。默认值为列表、字典或类实例等可变对象时，会产生与该规则不同的结果

不想在后续调用之间共享默认值时，应以如下方式编写函数

```py
def f(a, L=None):
    if L is None:
        L = []
    L.append(a)
    return L
```

### 关键字参数

```py
def func(a, b = 1, c = 2):
    print(a, b, c)

func(0)
func(0, c = 1)
func(a = 2, c = 1)
```

* 调用函数时可用 `kwarg=value` 形式的关键字参数
* 函数调用时，关键字参数必须跟在位置参数后面
* 关键字参数的顺序并不重要。这也包括必选参数
* 不能对同一个参数多次赋值
* 所有传递的关键字参数都必须匹配一个函数接受的参数

最后一个形参为 `**name` 形式时，接受一个字典，`**name` 形参可以与 `*name` 形参（下一小节介绍）组合使用（`*name` 必须在 `**name` 前面）
`*name` 形参接收一个元组，该元组包含形参列表之外的位置参数

```py
def func(a, *b, **c):
    print(a)
    for i in b:
        print(i)
    for k in c:
        print(k, c[k])

func(True, 1, 2, 3, i = "hello", j = "world")
```

### lambda

```
lambda_expr        ::=  "lambda" [parameter_list] ":" expression
lambda_expr_nocond ::=  "lambda" [parameter_list] ":" expression_nocond
```

lambda 创建匿名函数 如 `(lambda x: x ** 2)(5)` 创建一个匿名函数并调用

`lambda parameters: expression` 类似于如下方式定义函数


```py
def <lambda>(parameters):
     return expression
```
## 数据类型

### 内置类型

- `None`
- `numbers.Number`
  - `numbers.Integral`
    - `int`
    - `bool`
  - `numbers.Real`(`float`)
  - `numbers.Complex`(`complex`)
- 序列
  - 不可变序列
    - 字符串(`str`)
    - 元组(`tuple`)
    - 字节串(`bytes`)
    - 范围(`range`)
  - 可变序列
    - 列表(`list`)
    - 字节数组(`bytearray`)
- 集合
  - 集合(`set`)
  - 冻结集合(`frozenset`)
- 映射
  - 字典(`dict`)

### 数字类型

#### int

整数类型，拥有无限精度

构造函数 `int()` 用以构造 int 类型数字

int 类型的按位运算

| 运算 | 结果 |
| -- | -- |
| `x | y` | 按位或 |
| `x ^ y` | 按位异或 |
| `x & y` | 按位与 |
| `x << n`| 左移 (n >= 0) |
| `x >> n`| 右移 (n >= 0) |
| `~x`    | 按位取反 |

int 类型的方法，字面量调用如 `1.bit_length()` 是错误的语法，可改成 `(1).bit_length()`

#### bool 类型

bool 类型是两个常量对象之一: `True`, `False`，可用内置函数 `bool()` 将任意值转为 bool 类型

#### float 类型

一般使用C 语言的 `double` 来实现，浮点数构造函数 `float()`

#### complex 类型

复数包含实部和虚部，分别以一个浮点数表示。 要从一个复数 z 中提取这两个部分，可使用 `z.real` 和 `z.imag`

复数构造函数 `complex()`

### 数字类型的运算

| 运算 | 结果 |
| -- | -- |
| `x + y`   | 和 |
| `x - y`   | 差 |
| `x * y`   | 乘积 |
| `x / y`   | 商 |
| `x // y`  | 整除 |
| `x % y`   | 模 |
| `-x`      | 取反 |
| `+x`      | x 不变 |
| `abs(x)`  | 绝对值 |
| `int(x)`  | 转为整数 |
| `float(x)`| 转为浮点数 |
| `pow(x, y)`   | x 的 y 次幂 |
| `x ** y`      | x 的 y 次幂 |
| `divmod(x, y)`| `(x // y, x % y)` |
| `complex(re, im)` | 带有实部re, 虚部im(默认0) 的复数 |
| `c.conjugate()`   | 复数c 的共轭 |

### 序列类型

通用序列（可变与不可变序列）操作

| 运算 | 结果 |
| -- | -- |
| `x in s`      | 如果 s 中的某项等于 x 则结果为 True，否则为 False |
| `x not in s`  | 如果 s 中的某项等于 x 则结果为 False，否则为 True |
| `s + t`       | s 与 t 相拼接 |
| `s * n` 或 `n * s` | 相当于 s 与自身进行 n 次拼接 |
| `s[i]`        | 第 i 项（从0起） |
| `s[i:j]`      | s 从 i 到 j 的切片 |
| `s[i:j:k]`    | s 从 i 到 j 步长为 k 的切片 |
| `len(s)`      | s 的长度 |
| `min(s)`      | s 的最小项 |
| `max(s)`      | s 的最大项 |
| `s.index(x[, i[, j]])` | x 在 s 中首次出现项的索引号（索引号在 i 或其后且在 j 之前） |
| `s.count(x)`  | x 在 s 中出现的总次数 |

相同类型的序列支持比较操作

不可变序列类型普遍实现而可变序列类型未实现的唯一操作就是对 `hash()` 内置函数的支持

可变序列操作

| 运算 | 结果 |
| -- | -- |
| `s[i] = x`    | 将 s 的第 i 项替换为 x |
| `s[i:j] = t`  | 将 s 从 i 到 j 的切片替换为可迭代对象 t 的内容 |
| `del s[i:j]`  | 等同于 `s[i:j] = []` |
| `s[i:j:k] = t`| 将 `s[i:j:k]` 的元素替换为 t 的元素 |
| `del s[i:j:k]`| 从列表中移除 `s[i:j:k]` 的元素 |
| `s.append(x)` | 将 x 添加到序列的末尾 (等同于 `s[len(s):len(s)] = [x]`) |
| `s.clear()`   | 从 s 中移除所有项 (等同于 `del s[:]`) |
| `s.copy()`    | 创建 s 的浅拷贝 (等同于 `s[:]`) |
| `s.extend(t)` 或 `s += t` | 用 t 的内容扩展 s (基本上等同于 `s[len(s):len(s)] = t`) |
| `s *= n`      | 使用 s 的内容重复 n 次来对其进行更新 |
| `s.insert(i, x)` | 在由 i 给出的索引位置将 x 插入 s (等同于 `s[i:i] = [x]`) |
| `s.pop([i])`  | 提取在 i 位置上的项，并将其从 s 中移除，可选参数 i 默认为 -1，移除并返回最后一项 |
| `s.remove(x)` | 删除 s 中第一个 s[i] 等于 x 的项目 |
| `s.reverse()` | 就地将列表中的元素逆序 |

### 字符串

Python 中的字符串是 str 类型，是由 Unicode 码位构成的不可变序列

字符串字面量的三种写法

- 单引号: `'允许包含有 "双" 引号'`
- 双引号: `"允许包含有 '单' 引号"`
- 三重引号: `'''三重单引号''', """三重双引号"""`

使用三重引号的字符串可以跨越多行 —— 其中所有的空白字符都将包含在该字符串字面值中，由空格分隔的字符串字面量会转换成单个字符串

字符串构造函数 `str()`

对于字符串 s, s[i] 将产生长度为1 的字符串（Python 中没有字符类型），`s[i] == s[i:i+1]`

格式化字符串

- 格式字符串字面值(`f-string`): 以`f/F` 为前缀，`{}` 为标记
- printf 风格格式化字符串 `format % values`
- `str.format()`

```py
nums = 100
hello = "world"

print(f'nums = {nums}, hello {hello}')
print('nums = %d, hello %s' % (nums, hello))
print('nums = {}, hello {}'.format(nums, hello))

# nums = 100, hello world
```

### 元组

元组是不可变序列，通常用于储存异构数据的多项集，也被用于需要同构数据的不可变序列的情况

构建元组的方式

- 使用一对圆括号来表示空元组: `()`
- 使用一个后缀的逗号来表示单元组: `a,` 或 `(a,)`
- 使用以逗号分隔的多个项: `a, b, c` 或 `(a, b, c)`
- 使用内置的 `tuple()`: `tuple()` 或 `tuple(iterable)`

决定生成元组的其实是逗号而不是圆括号，圆括号只是可选的，生成空元组或需要避免语法歧义的情况除外

#### 元祖拆包(可迭代元素拆包)

```py
point = (1, 2)
x, y = point
y, x = x, y
"%d, %d" % point

t = (20, 8)
divmod(*t)

import os
_, filename = os.path.split('~/.ssh/id_rsa.pub')

a, b, *rest = range(5)
a, *body, c, d = range(5)
*head, b, c, d = range(5)

a, (b, c) = (1, (2, 3))
```

### 字节串

bytes 对象是由单个字节构成的不可变序列

bytes 字面量（类似str 字面量，有一个前缀b）

- 单引号: `b'同样允许嵌入 "双" 引号'`
- 双引号: `b"同样允许嵌入 '单' 引号"`
- 三重引号: `b'''三重单引号''', b"""三重双引号"""`

str, bytes 字面量通过前缀 r 禁用转义序列

bytes 构造函数 `bytes()`

### 范围

`range` 类型表示不可变的数字序列，通常用于在 for 循环中循环指定的次数

构造函数

- `range(stop)`
- `range(start, stop[, step])`

```py
>>> list(range(10))
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> list(range(1, 11))
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> list(range(0, 30, 5))
[0, 5, 10, 15, 20, 25]
>>> list(range(0, 10, 3))
[0, 3, 6, 9]
>>> list(range(0, -10, -1))
[0, -1, -2, -3, -4, -5, -6, -7, -8, -9]
>>> list(range(0))
[]
>>> list(range(1, 0))
[]
```

### 列表

列表是可变序列，通常用于存放同类项目的集合

构造列表的方式

- 使用一对方括号来表示空列表: `[]`
- 使用方括号，其中的项以逗号分隔: `[a]`, `[a, b, c]`
- 使用列表推导式: `[x for x in iterable]`
- 使用类型的构造器: `list()` 或 `list(iterable)`

#### 列表推导式

```py
symbols = '$¢£¥€¤'
beyond_ascii = [ord(s) for s in symbols if ord(s) > 127]
beyond_ascii2 = list(filter(lambda c: c > 127, map(ord, symbols)))
# [162, 163, 165, 8364, 164]

colors = ['black', 'white']
sizes = ['S', 'M', 'L']
tshirts = [(color, size) for color in colors for size in sizes]
```

#### 生成器表达式

```py
symbols = '$¢£¥€¤'
tuple(ord(symbol) for symbol in symbols)

import array
array.array('I', (ord(symbol) for symbol in symbols))

colors = ['black', 'white']
sizes = ['S', 'M', 'L']
for tshirt in ('%s %s' % (c, s) for c in colors for s in sizes):
    print(tshirt)
```

- 如果生成器表达式是一个函数调用过程中的唯一参数，那么不需要额外再用括号把它围起来
- 生成器表达式会在每次for循环运行时才生成一个组合

### 字节数组

`bytearray` 对象是 `bytes` 对象的可变对应物

`bytearray` 总是通过调用构造器来创建 `bytearray()`

### 集合

集合是由具有唯一性的 `hashable` 对象所组成的无序多项集

`set` 类型是可变的，其内容可以使用 `add()` 和 `remove()` 这样的方法来改变。由于是可变类型，它没有哈希值，且不能被用作字典的键或其他集合的元素

`frozenset` 类型是不可变并且为 `hashable`，其内容在被创建后不能再改变；因此它可以被用作字典的键或其他集合的元素

构造函数分别为 `set()` `frozenset()`

非空的 set (不是 frozenset) 还可以通过将以逗号分隔的元素列表包含于花括号之内来创建，例如: `{'jack', 'sjoerd'}`

### 字典

映射对象会将 `hashable` 值映射到任意对象，Python 提供 `dict` 映射类型，`dict` 属于可变对象

创建字典

- 字面量: 包含与 `{}` 内的 `key:value` 列表，如 `{'jack': 4098, 'sjoerd': 4127}`
- 字典推导式: `{}`, `{x: x ** 2 for x in range(10)}`
- 构造函数 `dict`: `dict()`, `dict([('foo', 100), ('bar', 200)])`, `dict(foo=100, bar=200)`

字典的操作

| 操作 | 说明 |
| -- | -- |
| `list(d)`     | 返回字典 d 中使用的所有键的列表 |
| `len(d)`      | 返回字典 d 中的项数 |
| `d[key]`      | 返回 d 中以 key 为键的项。 如果映射中不存在 key 则会引发 `KeyError` |
| `d[key]=value`| 将 d[key] 设为 value |
| `del d[key]`  | 将 d[key] 从 d 中移除。 如果映射中不存在 key 则会引发 `KeyError` |
| `key in d`    | 如果 d 中存在键 key 则返回 True，否则返回 False |
| `key not in d`| 等价于 `not key in d` |
| `iter(d)`     | 返回以字典的键为元素的迭代器。 这是 iter(d.keys()) 的快捷方式 |
| `clear()`     | 移除字典中的所有元素 |
| `copy()`      | 返回原字典的浅拷贝 |
| `fromkeys(iterable[, value])` | 使用来自 iterable 的键创建一个新字典，并将键值设为 value |
| `get(key[, default])` | 如果 key 存在于字典中则返回 key 的值，否则返回 default。 如果 default 未给出则默认为 `None`，因而此方法绝不会引发 `KeyError` |
| `items()`     | 返回由字典项(key-value)组成的一个新视图 |
| `keys()`      | 返回由字典键(key)组成的一个新视图 |
| `values()`    | 返回由字典值组成的一个新视图 |
| `pop(key[, default])` | 如果 key 存在于字典中则将其移除并返回其值，否则返回 default。 如果 default 未给出且 key 不存在于字典中，则会引发 KeyError |
| `popitem()`   | 从字典中移除并返回一个 (键, 值) 对。 键值对会按 LIFO 的顺序被返回 |
| `reversed(d)` | 返回一个逆序获取字典键的迭代器。 这是 reversed(d.keys()) 的快捷方式 |
| `setdefault(key[, default])` | 如果字典存在键 key ，返回它的值。如果不存在，插入值为 default 的键 key ，并返回 default 。 default 默认为 None |
| `update([other])` | 使用来自 other 的键/值对更新字典，覆盖原有的键 |
| `d | other`   | 合并 d 和 other 中的键和值来创建一个新的字典，两者必须都是字典。当 d 和 other 有相同键时， other 的值优先 |
| `d |= other`  | 用 other 的键和值更新字典 d ，other 可以是 `mapping` 或 `iterable` 的键值对。当 d 和 other 有相同键时， other 的值优先 |

### 其它内置函数

`enumerate(iterable, start=0)` 返回一个枚举对象

```py
lists = ["hello", "world"]
for i, v in enumerate(lists):
    print(f"lists[{i}] = {v}")
```

`zip(*iterables)` 创建一个聚合了来自每个可迭代对象中的元素的迭代器，返回一个元组的迭代器

- 其中的第 i 个元组包含来自每个参数序列或可迭代对象的第 i 个元素
- 当所输入可迭代对象中最短的一个被耗尽时，迭代器将停止迭代
- 当只有一个可迭代对象参数时，它将返回一个单元组的迭代器
- 不带参数时，它将返回一个空迭代器

```py
list1 = [1, 2]
list2 = ["hello", "world"]
for i in zip(list1, list2):
    print(i)

# (1, 'hello')
# (2, 'world')
```
## 流程控制语句

### 逻辑值检测

一个对象在默认情况下均被视为真值，除非当该对象被调用时其所属类定义了 `__bool__()` 方法且返回 `False` 或是定义了 `__len__()` 方法且返回零

以下对象为假值

- 常量 `None`, `False`
- 数值类型的零: `0, 0.0, 0j, Decimal(0), Fraction(0, 1)`
- 空的序列和多项集: `'', (), [], {}, set(), range(0)`

### 布尔运算

- x `or` y: 如果x 为假, 则y, 否则x（x 为真时短路）
- x `and` y: 如果x 为假, 则x, 否则y（x 为假时短路）
- `not` x: 如果x 为假, 则`True`, 否则`False`

### 比较运算符

| 运算 | 含义 |
| -- | -- |
| `<`   | 严格小于  |
| `<=`  | 小于或等于|
| `>`   | 严格大于  |
| `>=`  | 大于或等于|
| `==`  | 等于      |
| `!=`  | 不等于    |
| `is`  | 对象标识  |
| `is not` | 否定的对象标识 |
| `in`  | 成员运算检测 |
| `not in` | 否定的成员运算检测 |

比较运算符优先级相同，且高于布尔运算

比较运算可以任意串连；例如，x < y <= z 等价于 x < y and y <= z，前者的不同之处在于 y 只被求值一次（但在两种情况下当 x < y 结果为假值时 z 都不会被求值）

### if

```
if_stmt ::=  "if" assignment_expression ":" suite
             ("elif" assignment_expression ":" suite)*
             ["else" ":" suite]
```

```py
string = 'hello'
lists = ['hello', 'world']
if string in lists:
    print(string, "in", lists)
else:
    print(string, "not in", lists)
```

### while

```
while_stmt ::=  "while" assignment_expression ":" suite
                ["else" ":" suite]
```

```py
i = 0
lists = ['hello', 'world']
while i < len(lists):
    print(lists[i])
    i += 1
```

### for

```
for_stmt ::=  "for" target_list "in" expression_list ":" suite
              ["else" ":" suite]
```

```py
lists = ['hello', 'world']
for i in lists:
    print(i)
```
### break, continue

while, for 语句中的 break, continue 分别用以终止循环与跳过循环内剩余语句

### try

```
try_stmt  ::=  try1_stmt | try2_stmt
try1_stmt ::=  "try" ":" suite
               ("except" [expression ["as" identifier]] ":" suite)+
               ["else" ":" suite]
               ["finally" ":" suite]
try2_stmt ::=  "try" ":" suite
               "finally" ":" suite
```

```py
try:
    1 / 0
except ZeroDivisionError:
    print("division by zero")
```

### with

```
with_stmt ::=  "with" with_item ("," with_item)* ":" suite
with_item ::=  expression ["as" target]
```

```py
with open("test.txt", "r") as f:
    pass
```

### pass

`pass` 用以占位

```py
def f(arg): pass
class C: pass
```
## 容器

### `array.array`

```py
from array import array

arr = array('i', [14, 21, 5, 29])
```

| 类型码 | C类型 | Python 类型 | 最小字节大小 |
| -- | -- | -- | -- |
| `'b'` | `signed char`     | `int`     | 1 |
| `'B'` | `unsigned char`   | `int`     | 1 |
| `'u'` | `wchar_t`         | Unicode 字符 | 2 |
| `'h'` | `signed short`    | `int`     | 2 |
| `'H'` | `unsigned short`  | `int`     | 2 |
| `'i'` | `signed int`      | `int`     | 2 |
| `'I'` | `unsigned int`    | `int`     | 2 |
| `'l'` | `signed long`     | `int`     | 4 |
| `'L'` | `unsigned long`   | `int`     | 4 |
| `'q'` | `signed long long`| `int`     | 8 |
| `'Q'` | `unsigned long long`| `int`   | 8 |
| `'f'` | `float`           | `float`   | 4 |
| `'d'` | `double`          | `float`   | 8 |

### `collections.namedtuple`

`collections.namedtuple(typename, field_names, *, rename=False, defaults=None, module=None)`

- 返回一个新的元组子类，名为 typename 。这个新的子类用于创建类元组的对象，可以通过字段名来获取属性值，同样也可以通过索引和迭代获取值
- 创建一个具名元组需要两个参数，一个是类名，另一个是类的各个字段的名字。后者可以是由数个字符串组成的可迭代对象，或者是由空格分隔开的字段名组成的字符串
- 存放在对应字段里的数据要以一串参数的形式传入到构造函数中（注意，元组的构造函数却只接受单一的可迭代对象）
- 可以通过字段名或者位置来获取一个字段的信息

```py
import collections

Point = collections.namedtuple('point', ['x', 'y'])
a = Point(0, 1)
print(a)                # point(x=0, y=1)
print(Point._fields)    # ('x', 'y')

b = Point._make((1, 2))
print(b)                # point(x=1, y=2)

print(b._asdict())      # {'x': 1, 'y': 2}
```

`namedtuple` 专有属性

- `_fields` 属性是一个包含这个类所有字段名称的元组
- 用 `_make()` 通过接受一个可迭代对象来生成这个类的一个实例，它的作用跟 `Class(*data)` 是一样的
- `_asdict()` 把具名元组以 `collections.OrderedDict` 的形式返回
## 类

### 属性与实例化

```py
class MyClass:
    """A simple example class"""
    i = 12345

    def f(self):
        return 'hello world'

print(MyClass.i) # 12345
print(MyClass.f) # <function MyClass.f at 0x7f1c6e4b7280>
print(MyClass.__doc__) # A simple example class
```

类的属性引用: `obj.name`，类无 `name` 属性时，引发 `AttributeError`

类的实例化: `x = MyClass()`

### 构造函数

当定义了 `__init__()`时，类的实例化操作会自动为新创建的类实例发起调用 `__init__()`

init 函数还可以提供额外参数，例如

```py
def __init__(self):
    self.data = []

def __init__(self, x, y):
    self.x = x
    self.y = y
```

### 实例对象

实例对象的唯一操作是属性引用；有两种有效的属性名称：数据属性和方法

实例变量用于每个实例的唯一数据，而类变量用于类的所有实例共享的属性和方法

如果同样的属性名称同时出现在实例和类中，则属性查找会优先选择实例

```py
class MyClass:
    count = 0

    def __init__(self):
        # count += 1        # 错误，访问不存在的局部变量
        # self.count += 1   # 此变量为实例变量，先访问类变量 count 后创建实例变量 self.count
        MyClass.count += 1

x = MyClass()
print(MyClass.count)        # 1
print(x.count)              # 1 访问类变量
x.count = 100               # 创建实例变量
print(x.count)              # 100 访问实例变量
```

方法的第一个参数常常被命名为 `self`。 这也不过就是一个约定: `self` 这一名称在 Python 中绝对没有特殊含义

### 继承

```py
class Base:
    i = 0
    def func(self):
        print(self.i)

    def func2(self):
        self.i += 1

class Derived(Base):
    def func2(self):
        Base.func2(self)
        print(self.i)

x = Derived()
print(x.i)                          # 0 继承变量
x.func()                            # 0 继承函数
x.func2()                           # 1 覆盖函数

print(isinstance(x, Base))          # True
print(isinstance(x, Derived))       # True

print(issubclass(Base, Derived))    # False
print(issubclass(Derived, Base))    # True
```

`isinstance(obj, class)` 仅会在 `obj.__class__` 为 `class` 或某个派生自 `class` 的类时为 `True`

`issubclass(classA, classB)` 在 `classA` 为 `classB` 的子类是为 `True`

### 多重继承

`class Derived(Base1, Base2, ...)`

从父类所继承属性的操作是深度优先、从左至右的，当层次结构中存在重叠时不会在同一个类中搜索两次

如果某一属性在 DerivedClassName 中未找到，则会到 Base1 中搜索它，然后（递归地）到 Base1 的基类中搜索，如果在那里未找到，再到 Base2 中搜索，依此类推

真实情况比这个更复杂一些；方法解析顺序会动态改变以支持对 `super()` 的协同调用。
这种方式在某些其他多重继承型语言中被称为后续方法调用，它比单继承型语言中的 `super` 调用更强大。

### 特殊属性

| 属性 | 说明 |
| -- | -- |
| `__init__`    | 构造函数 |
| `__next__`    | 从容器中返回下一项 |
| `__iter__`    | 返回迭代器对象本身 |
| `__dict__`    | 一个字典或其他类型的映射对象，用于存储对象的（可写）属性 |
| `__getitem__` | 调用此方法以实现 `self[key]` 的求值 |
| `__getattr__` | 此方法应当返回（找到的）属性值或是引发一个 `AttributeError` 异常 |
| `__del__`     | 析构函数 |
| `__call__`    | 此方法会在实例作为一个函数被“调用”时被调用 |
| `__class__`   | 类实例所属的类 |
| `__bases__`   | 由类对象的基类所组成的元组 |
| `__name__`    | 类、函数、方法、描述器或生成器实例的名称 |
| `__repr__`    | 输出一个对象的“官方”字符串表示。如果可能，这应类似一个有效的 Python 表达式，能被用来重建具有相同取值的对象 |
| `__str__`     | 生成一个对象的“非正式”或格式良好的字符串表示；内置类型 `object` 所定义的默认实现会调用 `object.__repr__()` |
| `__bool__`    | 调用此方法以实现真值检测以及内置的 `bool()` 操作 |

```py
class MyClass:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def __repr__(self):
        return f"MyClass({self.x}, {self.y})"

    def __str__(self):
        return f"self.x = {self.x}, self.y = {self.y}"

    def __call__(self, x, y):
        self.x = x
        self.y = y

    def __del__(self):
        print("__del__ called")

    def __getitem__(self, key):
        if key == 0 or key == 'x':
            return self.x
        elif key == 1 or key == 'y':
            return self.y
        else:
            return None

    def __bool__(self):
        return self.x != 0 or self.y != 0

print(MyClass.__class__)    # <class 'type'>
print(MyClass.__name__)     # MyClass
print(MyClass.__bases__)    # (<class 'object'>,)

x = MyClass()
print(x)                    # self.x = 0, self.y = 0
print(str(x))               # self.x = 0, self.y = 0
print(repr(x))              # MyClass(0, 0)

print(bool(x))              # False
x(1, 2)
print(x)                    # self.x = 1, self.y = 2
print(bool(x))              # True

print(x["x"], x["y"])       # 1 2

x = None                    # __del__ called
```
## ast - 抽象语法树

ast 模块帮助 Python 程序处理 Python 语法的抽象语法树

### 节点与函数

`ast.AST` AST 节点类的基类

`ast.parse()` 把源码解析为AST节点

`ast.unparse()` 反解析为字符串

```py
import ast

stat = """
i = 1
i += 1
import ABC
from X import x
def func():
    pass
"""

tree = ast.parse(stat)
for node in tree.body:
    if isinstance(node, ast.Assign):
        print("ast.Assign", end = ': ')
        for target in node.targets:
            if isinstance(target, ast.Name):
                print(f"target.id is {target.id}")
    elif isinstance(node, ast.Expr):
        print("ast.Expr", end = ': ')
    elif isinstance(node, ast.Import) or isinstance(node, ast.ImportFrom):
        print("ast.Import or ast.ImportFrom", end = ': ')
    else:
        print(node)

    print(ast.unparse(node))
```

### ast.NodeTransformer

`NodeTransformer` 将遍历抽象语法树并使用 `visitor` 方法的返回值去替换或移除旧节点。
如果 `visitor` 方法的返回值为 `None` , 则该节点将从其位置移除，否则将替换为返回值。当返回值是原始节点时，无需替换

```py
import ast

class RewriteName(ast.NodeTransformer):
    def __init__(self, data = [], prefix = 'self'):
        self.data = data
        self.prefix = prefix

    def visit_Name(self, node):
        if node.id in self.data:
            return ast.copy_location(ast.Name(id = f'{self.prefix}.{node.id}', ctx = node.ctx), node)
        else:
            return ast.Subscript(
                value = ast.Name(id = 'data', ctx = ast.Load()),
                slice = ast.Constant(value = node.id),
                ctx = node.ctx
            )


stat = """
i = 1
a = i + 1
"""

tree = ast.parse(stat)
tree = ast.fix_missing_locations(RewriteName(data = "i").visit(tree))
print(ast.unparse(tree))

# self.i = 1
# data['a'] = self.i + 1
```
## Beautiful Soup

[Beautiful Soup](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/) 是第三方库，使用前需安装(`bs4`)

Beautiful Soup 用于从 HTML 或 XML 文件中提取数据

```py
from bs4 import BeautifulSoup

html_doc = """
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title"><b>The Dormouse's story</b></p>

<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>

<p class="story">...</p>
"""

soup = BeautifulSoup(html_doc, "html.parser")
print(soup.prettify())
print(soup.head, soup.title)

# 获取第一个 tag
print(soup.body.a)
# 搜索所有的 tag
for a in soup.find_all("a"):
    print(a)

head = soup.head
title = head.contents[0]
print(title)

for child in title.children:
    print(child)

sisters = soup.select(".sister")
for sister in sisters:
    print(sister.get_text())
```

## json, yaml 模块

yaml 不属于 python 标准库，需安装 [pyyaml](https://pypi.org/project/PyYAML/)

```py
import json
import yaml

data = {
    "a": 0,
    "b": True,
    "c": [1, 2, 3],
    "d": {
        "e": None,
    }
}

print(json.dumps(data))
# 紧凑编码
# {"a": 0, "b": true, "c": [1, 2, 3], "d": {"e": null}}

print(json.loads('["foo", {"bar":["baz", null, 1.0, 2]}]'))
# 解码
# ['foo', {'bar': ['baz', None, 1.0, 2]}]

print(yaml.dump(data))
# 编码
'''
a: 0
b: true
c:
- 1
- 2
- 3
d:
  e: null
'''

print(yaml.load('''
a: 1
b:
  c: 3
  d: 4
e:
''', Loader = yaml.Loader))
# 解码
# {'a': 1, 'b': {'c': 3, 'd': 4}, 'e': None}
```

json 命令行工具

`python -m json.tool` 读取标准输入，验证 json 并美化输出
## logging 日志库

### 记录器对象 `logging.Logger`

```py
import logging

logger = logging.getLogger(__name__)
handler = logging.StreamHandler()
logger.addHandler(handler)

logger.setLevel(logging.INFO)
# 日志等级小于 level 会被忽略
# 创建记录器时，级别默认设置为 NOTSET （当记录器是根记录器时，将处理所有消息；如果记录器不是根记录器，则将委托给父级）
# 根记录器的默认级别为 WARNING

logger.debug("debug log")
logger.info("info log")
logger.warning("warning log")
logger.error("error log")
logger.critical("critical log")

logger.log(logging.WARNING, "warning log")

print(logger.isEnabledFor(logging.DEBUG))
print(logger.isEnabledFor(logging.INFO))
```

### 日志级别

| 级别 | 数值 |
| -- | -- |
| CRITICAL  | 50 |
| ERROR     | 40 |
| WARNING   | 30 |
| INFO      | 20 |
| DEBUG     | 10 |
| NOTSET    | 0  |

### 处理器对象 `logging.Handler`

```py
handler = logging.FileHandler('output.log')
handler.setLevel(logging.ERROR)
logger.addHandler(handler)
logger.setLevel(logging.INFO)
# 日志级别为 error

logger.debug("debug log")
logger.info("info log")
logger.warning("warning log")
logger.error("error log")
logger.critical("critical log")
```

### 格式器对象 `logging.Formatter`

```py
import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
formatter = logging.Formatter(fmt='%(asctime)s - %(name)s - %(levelname)s - %(message)s', datefmt='%Y/%m/%d %H:%M:%S')
handler = logging.StreamHandler()
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.info("info log")
```
## requests - http 库

[requests](https://docs.python-requests.org/zh_CN/latest/) 不是 Python 标准库，使用前需安装

```py
import requests

# 发送请求
r = requests.get(url)
r = requests.post(url, data = {'key': 'value'})
r = requests.put(url, data = {'key': 'value'})
r = requests.delete(url)
r = requests.head(url)
r = requests.options(url)

# 响应
r.url                       # 编码后的 url
r.status_code               # 状态码
r.headers['content-type']   # 响应头
r.encoding                  # 编码
r.text                      # 响应内容
r._content                  # 二进制响应内容
r.json()                    # json 响应内容，json 解码失败则抛异常
r.raw                       # 原始响应内容
r.cookies                   # cookies

# 请求头
headers = {'user-agent': 'my-app/0.0.1'}
r = requests.get(url, headers=headers)

# 发送 cookies
cookies = dict(cookies_are='working')
r = requests.get(url, cookies=cookies)

# 超时
requests.get(url, timeout=0.001)
```
## sqlite3

Python 内置了 sqlite3 数据库的接口

```py
import sqlite3

conn = sqlite3.connect('test.db')
cur = conn.cursor()

cur.execute('''create table IF NOT EXISTS user
                (name string, passwd string, email string, PRIMARY KEY (name))''')
cur.execute('''insert into user values('abc', '123', 'abc@example.com')''')
conn.commit()

t = ('abc', )
cur.execute('''select * from user where name = ?''', t)
print(cur.fetchone())

conn.close()
```
## url 处理模块

```py
urllib.request.urlopen(url, data=None, [timeout, ]*, cafile=None, capath=None, cadefault=False, context=None)
# url 为字符串或 Request 对象
# 对于 HTTP 与 HTTPS url，返回 http.client.HTTPResponse 对象

urllib.parse.urlparse(urlstring, scheme='', allow_fragments=True)
# 解析 url
```

```python
from urllib import request

url = 'http://liph.ink'
with request.urlopen(url) as res:
    print(res.status, res.reason)
    for k, v in res.getheaders():
        print(k, v)

    data = res.read()
    print(data.decode('utf-8'))

req = request.Request(url)
with request.urlopen(req) as res:
    print(res.read().decode('utf-8'))


from urllib import parse

user = 'abc'
passwd = '123'
data = parse.urlencode([
    ('username', user),
    ('passwd', passwd)
])

req = request.Request(url)
req.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0')
with request.urlopen(req, data = data.encode('utf-8')) as res:
    print(res.status, res.reason)

```

