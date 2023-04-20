# 词法分析工具 lex

`lex [OPTIONS] [FILE]...` 生成词法分析器的程序

`flex`: [fast lex](https://github.com/westes/flex)

Linux 下 lex 一般是 flex 的链接

lex 源文件格式

```lex
Definitions
%%
Rules
%%
User Subroutines
```

可细分为如下结构

```lex
%{
part 1
%}
part 2
%%
part 3
%%
part 4
```

第一个 `%%` 是必要的，用以标记规则的开头；第二个 `%%` 只有定义用户子程序才需要

`part1` 通常包括一些C语句，将被复制到生成代码的开头，`part4` 则被复制到结尾

`part2` 包含命名正则表达式的定义，如

```lex
letter      [a-zA-Z]
digit       [0-9]
identifier  {letter}({letter}|{digit})*
```

可以在`{}` 中使用这些正则表达式定义

`part3` 定义`yylex()` 的规则，写法如下

```lex
R     Action
```

* `R` 是正则表达式， `Action` 是C代码， 在`Action` 中 `return n` 使 `yylex()` 返回 n 代表的记号(`token`)
* `Action` 中的多条语句需要以 `{}` 包裹
* 如果多个正则表达式匹配同一字符串，会执行第一个正则表达式定义的 `Action`
* 最长匹配原则

## flex 正则表达式

* `[abcd]` 等价于 `(a|b|c|d)`
* `[0-9]` 中括号中的的破折号表示一个字符范围
* `[^abcd]` 除abcd 外的任意字符
* `.`  除了`\n`， 等价于 `[^\n]`
* `A*` 0或多个A
* `A+` 1或多个A， 等价于`AA*`
* `A?` 0或1个A
* `A/B` A，但仅限跟随B 的情况
* `"string"` 引号中原本的字符串string

## 一个例子

`lex 源文件 test.l`

```lex
%%
.|\n ECHO;
```

执行命令 `lex test.l` 会生成 `lex.yy.c` (-o 指定生成的文件名， -t 在标准输出打印，选项需在文件名前指定)

`main.c`

```c
extern int yylex();

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    return 0;
}
```

编译 `gcc main.c lex.yy.c`, 生成的程序可以将标准输入复制到标准输出

也可以把这两个函数加到 `lex` 源文件的用户子程序定义区域，直接编译 `lex.yy.c`

## `lex` 宏，变量与函数

* `yyin`, `FILE *` 类型，在调用`yylex()` 之前保存用来读取的文件流，默认是标准输入
* `yyout`, `FILE *` 类型，输出文件流，默认是标准输出
* `int yylex()`, 从`yyin` 读入并扫描符号
* `int yywrap()`, `yylex()` 结束后会调用`yywrap`, `yywrap`返回1(非0)意味着文件结束；返回0则扫描继续。
* `yytext`, `char *` 类型，保存当前匹配的字符串，以 null 结尾
* `yyleng`, `int` 类型，`yytext`的长度
* `yylval`, 记号(token) 属性，`YYSTYPE`类型，可行自定义，如

```c
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED
typedef union {
    const char* strval;
    int         ival;
} YYSTYPE;
#endif
```

`main.c` 改为如下代码

```c
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;

int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        yyin = stdin;
        yylex();
    } else {
        for (int i = 1; i < argc; i++) {
            yyin = fopen(argv[i], "r");
            if (yyin == NULL) {
                fprintf(stderr, "file %s: open failed.\n", argv[i]);
                exit(1);
            }
            yylex();
        }
    }
    return 0;
}
```

则编译出的程序类似于`cat` 命令
