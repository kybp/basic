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

static FILE *old_yyin = NULL;

static line lst;
static statement current_statement;

%}

%union {
    int integer;
    double real;
    char *string;
}

%token ADD SUB MUL DIV EXPT
%token GOSUB GOTO IF LET PRINT RETURN
%token LIST LOAD NEW RUN SAVE
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
| line INTEGER statement EOL {
    add_line(&lst, $2, &current_statement);
 }
| line command   EOL { /* work is done in the command */ }
| line statement EOL { eval_stmt(&current_statement, NULL); }
;

command: list_stmt
       | load_stmt
       | new_stmt
       | run_stmt
       | save_stmt
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

list_stmt: LIST {
    write_listing(&lst, stdout);
 }
;

load_stmt: LOAD STRING {
    reset_listing(&lst);
    old_yyin = yyin;
    yyin = fopen($2, "r");
 }
;

new_stmt: NEW {
    reset_listing(&lst);
 }
;

return_stmt: RETURN {
    current_statement.command = RETURN;
 }
;

run_stmt: RUN {
    eval_listing(&lst);
 }
;

save_stmt: SAVE STRING {
    save_listing(&lst, $2);
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
    yyparse();
}
