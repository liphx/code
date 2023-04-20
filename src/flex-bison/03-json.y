/*
 * bison -dy 03-json.y && flex 03-json.l && gcc y.tab.c lex.yy.c
 */

%{
#include <stdio.h>
extern int yylex();
extern int yylineno;
void yyerror(char *s);
%}

%token STRING NUMBER TRUE FALSE null

%%

json: value
    ;

object: '{' pairs '}'
    ;

pairs: /* empty */
    |
    pair
    |
    pairs ',' pair
    ;

pair: STRING ':' value
    ;

value: object
    |
    array
    |
    STRING
    |
    NUMBER
    |
    TRUE
    |
    FALSE
    |
    null
    ;

values: /* empty */
    |
    value
    |
    values ',' value
    ;

array: '[' values ']'
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}

int main() {
    yyparse();
}
