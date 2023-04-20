/*
 * bison -dy 02-calculator.y && flex 02-calculator.l && gcc y.tab.c lex.yy.c -lfl
 */

%{
#include "y.tab.h"
%}

%%

"//".*  /* ignore comments */
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"("     { return OP; }
")"     { return CP; }
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
0[xX][a-fA-F0-9]+ {
            yylval = strtol(yytext, NULL, 16);
            return NUMBER;
        }
\n      { return EOL; }
[ \t]   /* ignore */
.       { printf("Mystery character %s\n", yytext); }

%%
