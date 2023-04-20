/*
 * bison -dy 03-json.y && flex 03-json.l && gcc y.tab.c lex.yy.c
 */

%option noyywrap yylineno

%{
#include "y.tab.h"
%}

escape      \\["\\/bfnrt]|\\u[0-9a-fA-F]{4}
noescape    [^\"\\]
integer     0|[1-9][0-9]*
fraction    \.[0-9]+
exponent    [eE][-+]?[0-9]+
whitespace  [ \t\r\n]

%%

true                                { return TRUE; }
false                               { return FALSE; }
null                                { return null; }
[{}\[\],:]                          { return yytext[0]; }
\"({noescape}|{escape})*\"          { return STRING; }
-?{integer}{fraction}?{exponent}?   { return NUMBER; }
{whitespace}                        { /* ignore whitespace */ }
.                                   { fprintf(stderr, "unknown character: %s\n", yytext); }

%%
