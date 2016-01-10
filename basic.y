%{
#include <ctype.h>
#include <math.h>
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

%token REAL_CAST ROUND
%token GOSUB GOTO IF LET PRINT RETURN
%token LIST LOAD NEW RUN SAVE
%token LT LE EQ GE GT NE
%token COMMA SEMI LPAREN RPAREN EOL

%left ADD SUB
%left MUL DIV
%right EXPT

%type <integer> int_expr
%type <real> real_expr
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
         | print_stmt
         | return_stmt
;

int_expr: INTEGER
        | ROUND LPAREN REAL RPAREN { $$ = (int)($3 + 0.5); }
        | int_expr ADD int_expr { $$ = $1 + $3; }
        | int_expr SUB int_expr { $$ = $1 - $3; }
        | int_expr MUL int_expr { $$ = $1 * $3; }
        | int_expr DIV int_expr { $$ = $1 / $3; }
        ;

real_expr: REAL
         | REAL_CAST LPAREN INTEGER RPAREN { $$ = (double)$3; }
         | real_expr ADD real_expr  { $$ = $1 + $3; }
         | real_expr SUB real_expr  { $$ = $1 - $3; }
         | real_expr MUL real_expr  { $$ = $1 * $3; }
         | real_expr DIV real_expr  { $$ = $1 / $3; }
         | real_expr EXPT real_expr { $$ = pow($1, $3); }
         ;

/* Commands */

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

run_stmt: RUN {
    eval_listing(&lst);
 }
;

save_stmt: SAVE STRING {
    save_listing(&lst, $2);
 }
;

/* Statements */

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

print_stmt: PRINT int_expr {
    current_statement.command = PRINT;
    current_statement.type = INTEGER;
    current_statement.arg1.integer = $2;
 }
| PRINT STRING {
    current_statement.command = PRINT;
    current_statement.type = STRING;
    current_statement.arg1.string = $2;
 }
| PRINT real_expr {
    current_statement.command = PRINT;
    current_statement.type = REAL;
    current_statement.arg1.real = $2;
 }
| PRINT {
    current_statement.command = PRINT;
    current_statement.type = NOTHING;
  }
;

return_stmt: RETURN {
    current_statement.command = RETURN;
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
    return 0;
}
