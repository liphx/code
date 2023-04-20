# 语法分析工具 yacc

`yacc`: (Yet Another Compiler Compiler), 创建 `LALR(1)` 解析器

Linux 下一般使用 [Bison](https://www.gnu.org/software/bison/manual/bison.html) 替代 `yacc`。此外，还有`byacc` (Berkeley Yacc)，和 `Bison` 一样，提供兼容的 `yacc`工具。最重要的，`Bison`, `byacc`是 `free` 的，`yacc` 是`AT&T` 的专有软件

`bison [OPTION]... FILE` (通过`-y` 或 `--yacc` 兼容 yacc)

## Bison 源文件格式

类似`lex`源文件(`.l`), `bison` 源文件(`.y`) 也如下结构

```yacc
%{
part1 - 复制到生成的C文件的开头
%}
part2 - 词法声明
%%
part3 - 翻译规则
%%
part4 - 复制到生成的C文件的结尾
```

## 一个例子

`test.l`

```lex
%{
#include "y.tab.h"
%}

%%
-?[0-9]+        {   yylval = atoi(yytext);
                    return NUMBER;
                }
"+"             {   return PLUS; }
"\r"            {   ; }
"\n"            {   ; }
[ \t]+          {   ; }

%%

int yywrap()
{
    return 1;
}
```

`test.y`

```yacc
%{
#include <stdio.h>
extern int yylex();
void yyerror(char const *str);
typedef int YYSTYPE;
%}

%token NUMBER PLUS
%%

line : line expression { printf("%d\n", $2); }
     |
     ;

expression : expression PLUS expression { $$ =  $1 + $3; }
           | NUMBER
           ;
%%

void yyerror(char const *str)
{
    fprintf(stderr, "%s\n", str);
}
```

`main.c`

```c
extern int yyparse();

int main()
{
    yyparse();
    return 0;
}
```

编译执行，得到只支持加法的简易计算器

```shell
$ bison -d -y test.y # 生成 y.tab.c  y.tab.h
$ flex test.l        # 生成 lex.yy.c
$ gcc main.c y.tab.c lex.yy.c -o main
$ cat test.txt
1 + 2
7 + -8
$ ./main < test.txt
3
-1
```
