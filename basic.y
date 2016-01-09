%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "listing.h"
#include "stack.h"

#define MAX_LINE 1024

void yyerror(char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

void drop_number(char *from, char *to);

union argument {
    int integer;
    char *string;
};

static int current_line;
static stack st;
static line lst;

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
%token LIST SAVE
%token LT LE EQ GE GT NE
%token COMMA SEMI LPAREN RPAREN EOL

%token <string>  PROG_LINE
%token <integer> INTEGER
%token <real>    REAL
%token <string>  STRING
%token <string>  INT_VAR
%token <string>  REAL_VAR
%token <string>  STR_VAR

%%

line: /* nothing */
| line STRING EOL { puts($2); }
| line PROG_LINE EOL {
    char buf[MAX_LINE];
    drop_number($2, buf);
    add_line(&lst, atoi($2), buf);
 }
| line statement EOL { eval(); }
;

statement: gosub_stmt
         | goto_stmt
         | list_stmt
         | return_stmt
         | save_stmt
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

list_stmt: LIST {
    current_statement.command = LIST;
 }
;

return_stmt: RETURN {
    current_statement.command = RETURN;
 }
;

save_stmt: SAVE STRING {
    current_statement.command = SAVE;
    current_statement.arg1.string = $2;
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
    case LIST:
        write_listing(&lst, stdout);
    case RETURN:
        current_line = pop(&st);
        break;
    case SAVE:
        save_listing(&lst, current_statement.arg1.string);
        break;
    }
}

void drop_number(char *from, char *to)
{
    while (isdigit(*from++))
        ; /* first non-digit is a space */
    strcpy(to, from);
}
