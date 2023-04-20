/*
 * bison -dy 02-calculator.y && flex 02-calculator.l && gcc y.tab.c lex.yy.c -lfl
 */

%{
#include <stdio.h>
extern int yylex();
extern void yyerror(char *);
%}

%token NUMBER ADD SUB MUL DIV EOL
%token OP CP

%%

/* start symbol, match the entire input */
calclist: /* nothing */
    | calclist exp EOL  { printf("ans = %d\n> ", $2); }
    | calclist EOL { printf("> "); }
    ;

exp: factor /* default: $$ = $1 */
    | exp ADD factor    { $$ = $1 + $3; }
    | exp SUB factor    { $$ = $1 - $3; }
    ;

factor: term
    | factor MUL term   { $$ = $1 * $3; }
    | factor DIV term   { $$ = $1 / $3; }
    ;

term: NUMBER
    | OP exp CP         { $$ = $2; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char **argv) {
    printf("> ");
    yyparse();
}
