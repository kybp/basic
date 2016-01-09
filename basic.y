%{
#include <stdio.h>

void yyerror (char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

union argument {
    int integer;
    char *string;
};

struct {
    int command;
    union argument arg1;
    union argument arg2;
} current_statement;

%}

%union {
    int integer;
    double real;
    char *string;
}

%token ADD SUB MUL DIV EXPT
%token GOSUB GOTO IF LET PRINT RETURN
%token LT LE EQ GE GT NE
%token COMMA SEMI LPAREN RPAREN EOL

%token <integer> INTEGER
%token <real>    REAL
%token <string>  STRING
%token <string>  INT_VAR
%token <string>  REAL_VAR
%token <string>  STR_VAR

%%

line: /* nothing */
| line INTEGER statement EOL
| line statement EOL
;

statement: gosub_stmt
         | goto_stmt
;

gosub_stmt: GOSUB INTEGER {
    current_statement.command = GOSUB;
    current_statement.arg1.integer = $2;
 }
;

goto_stmt: GOTO INTEGER {
    current_statement.command = GOTO;
    current_statement.arg1.integer = $2;
 }
;

%%

int main(void)
{
    yyparse();
}
