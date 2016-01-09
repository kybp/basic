%{
#include <stdio.h>

void yyerror (char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

%}

%union {
    int integer;
    double real;
    char *string;
}

%token ADD SUB MUL DIV EXPT
%token GOSUB GOTO IF LET PRINT RETURN THEN
%token LT LE EQ GE GT NE
%token COMMA SEMI LPAREN RPAREN EOL

%token <integer> INTEGER
%token <real>    REAL
%token <string>  STRING
%token <string>  INT_VAR
%token <string>  REAL_VAR
%token <string>  STR_VAR

%%

line: statement EOL

statement: GOSUB INTEGER { printf("gosub %d\n", $2); };

%%

int main(void)
{
    yyparse();
}
