%option noyywrap

%{
static void comment();
%}

%%

"//".*
"/*"    comment();
\n|.    ECHO;

%%

/* if compiler by g++, use yyinput */
void comment() {
    int c;
    while ((c = input()) != 0) {
        if (c == '*') {
            while ((c = input()) == '*') ;
            if (c == '/') return;
            if (c == 0) break;
        }
    }
    fprintf(stderr, "error: unterminated comment\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    yylex();
}
