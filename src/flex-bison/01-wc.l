/*
 * like unix wc
 * flex 01-wc.l && gcc lex.yy.c -lfl
 * read form stdin
 */

%{
int chars = 0;
int words = 0;
int lines = 0;
%}

%%

[^ \t\n\r\f\v]+     { words++; chars += strlen(yytext); }
\n                  { chars++; lines++; }
.                   { chars++; }

%%

int main() {
    yylex();
    printf("lines: %d, words: %d, chars: %d\n", lines, words, chars);
}
