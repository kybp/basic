%{
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "listing.h"
#include "stack.h"
#include "symtab.h"

extern FILE *yyin;
int yylex(void);

void yyerror(char const *s) 
{
    fprintf(stderr, "%s\n", s);
}

static FILE *old_yyin = NULL;

static line lst;
static statement current_statement;
static symtab sym;

%}

%union {
    int integer;
    double real;
    char *string;
    struct expr *expr;
}

%token REAL_CAST ROUND
%token GOSUB GOTO IF LET PRINT RETURN
%token LIST LOAD NEW RUN SAVE
%token LT LE EQ GE GT NE
%token COMMA SEMI LPAREN RPAREN EOL

%left ADD SUB
%left MUL DIV
%right EXPT

%type <expr> int_expr
%type <expr> real_expr
%type <expr> str_expr

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
| line statement EOL { eval_stmt(&current_statement, NULL, &sym); }
;

command: list_stmt
       | load_stmt
       | new_stmt
       | run_stmt
       | save_stmt
       ;

statement: gosub_stmt
         | goto_stmt
         | if_stmt
         | let_stmt
         | print_stmt
         | return_stmt
         ;

/* Expressions */

int_expr: INTEGER { $$ = new_int_expr($1); }
        | INT_VAR { $$ = new_var_expr(INT_VAR, $1); }
        ;

real_expr: REAL     { $$ = new_real_expr($1); }
         | REAL_VAR { $$ = new_var_expr(REAL_VAR, $1); }
         ;

str_expr: STRING  { $$ = new_str_expr($1); }
        | STR_VAR { $$ = new_var_expr(STR_VAR, $1); }
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
    eval_listing(&lst, &sym);
 }
;

save_stmt: SAVE STRING {
    save_listing(&lst, $2);
 }
;

/* Statements */

gosub_stmt: GOSUB INTEGER {
    current_statement.command = GOSUB;
    current_statement.arg1 = new_int_expr($2);
 }
;

goto_stmt: GOTO INTEGER {
    current_statement.command = GOTO;
    current_statement.arg1 = new_int_expr($2);
 }
;

if_stmt: IF int_expr GOTO INTEGER {
    current_statement.command = IF;
    current_statement.arg1 = $2;
    current_statement.arg2 = new_int_expr($4);
 }
;

let_stmt: LET INT_VAR EQ int_expr {
    current_statement.command = LET;
    current_statement.arg1 = new_var_expr(INTEGER, $2);
    current_statement.arg2 = $4;
 }
| LET REAL_VAR EQ real_expr {
    current_statement.command = LET;
    current_statement.arg1 = new_var_expr(REAL, $2);
    current_statement.arg2 = $4;
 }
| LET STR_VAR EQ str_expr {
    current_statement.command = LET;
    current_statement.arg1 = new_var_expr(STRING, $2);
    current_statement.arg2 = $4;
 }
;

print_stmt: PRINT int_expr {
    current_statement.command = PRINT;
    current_statement.arg1 = $2;
 }
| PRINT str_expr {
    current_statement.command = PRINT;
    current_statement.arg1 = $2;
 }
| PRINT real_expr {
    current_statement.command = PRINT;
    current_statement.arg1 = $2;
 }
| PRINT {
    current_statement.command = PRINT;
    current_statement.arg1 = NULL;
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
