%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listing.h"
#include "stack.h"

#define MAX_LINE 1024

extern FILE *yyin;
int yylex(void);

void yyerror(char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

void drop_number(char *from, char *to);

static FILE *old_yyin = NULL;

static int current_line;
static stack st;
static line lst;

statement current_statement;

void eval(void);
%}

%union {
    int integer;
    double real;
    char *string;
}

%token ADD SUB MUL DIV EXPT
%token GOSUB GOTO IF LET PRINT RETURN
%token LIST LOAD NEW SAVE
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
| line PROG_LINE EOL {
    char buf[MAX_LINE];
    drop_number($2, buf);
    add_line(&lst, atoi($2), buf, &current_statement);
 }
| line statement EOL { eval(); }
;

statement: gosub_stmt
         | goto_stmt
         | list_stmt
         | load_stmt
         | new_stmt
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

load_stmt: LOAD STRING {
    current_statement.command = LOAD;
    current_statement.arg1.string = $2;
 }
;

new_stmt: NEW {
    current_statement.command = NEW;
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

int yywrap(void)
{
    if (old_yyin) {
        fclose(yyin);
        yyin = old_yyin;
        old_yyin = NULL;
        return 0;
    } else {
        return 1;
    }
}

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
        break;
    case LOAD:
        reset_listing(&lst);
        old_yyin = yyin;
        yyin = fopen(current_statement.arg1.string, "r");
        break;
    case NEW:
        reset_listing(&lst);
        break;
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
