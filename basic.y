%{
#include <stdio.h>
#include "stack.h"

void yyerror (char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

union argument {
    int integer;
    char *string;
};

static int current_line;
static stack st;

struct {
    int command;
    union argument arg1;
    union argument arg2;
} current_statement;

void eval(void);
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
| line statement EOL { eval(); }
;

statement: gosub_stmt
         | goto_stmt
         | return_stmt
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

return_stmt: RETURN {
    current_statement.command = RETURN;
 }
;

%%

int main(void)
{
    init_stack(&st, 16);
    yyparse();
}

void eval() {
    switch(current_statement.command) {
    case GOSUB:
        push(&st, current_line);
    case GOTO:
        current_line = current_statement.arg1.integer;
        break;
    case RETURN:
        current_line = pop(&st);
    }
}
