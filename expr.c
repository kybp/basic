#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
#include "expr.h"
#include "symtab.h"

#define NOTHING 0

void eval(expr *e, symtab *table)
{
    if (e->op == NOTHING) return;

    switch (e->op) {
    case ADD:
        eval(e->arg1, table);
        eval(e->arg2, table);
        e->op = NOTHING;
        if (e->arg1->type == INTEGER && e->arg2->type == INTEGER) {
            e->type = INTEGER;
            e->val.integer = e->arg1->val.integer + e->arg2->val.integer;
        } else if (e->arg1->type == INTEGER && e->arg2->type == REAL) {
            e->type = REAL;
            e->val.real = (double)e->arg1->val.integer + e->arg2->val.real;
        } else if (e->arg1->type == REAL && e->arg2->type == INTEGER) {
            e->type = REAL;
            e->val.real = e->arg1->val.real + (double)e->arg2->val.integer;
        } else if (e->arg1->type == REAL && e->arg2->type == REAL) {
            e->type = REAL;
            e->val.real = e->arg1->val.real + e->arg2->val.real;
        }
    }
}

expr *new_int_expr(int n)
{
    expr *e = (expr *)malloc(sizeof(expr));
    e->type = INTEGER;
    e->val.integer = n;
    return e;
}

expr *new_real_expr(double n)
{
    expr *e = (expr *)malloc(sizeof(expr));
    e->type = REAL;
    e->val.real = n;
    return e;
}

expr *new_str_expr(char *s)
{
    expr *e = (expr *)malloc(sizeof(expr));
    char *t = (char *)malloc(strlen(s) + 1);
    strcpy(t, s);
    e->type = STRING;
    e->val.string = t;
    return e;
}

expr *new_var_expr(int type, char *name)
{
    expr *e = (expr *)malloc(sizeof(expr));
    char *s = (char *)malloc(strlen(name) + 1);
    strcpy(s, name);
    e->type = type;
    e->val.string = s;
    return e;
}

void write_expr(FILE *f, expr *e)
{
    (void)e;
    fprintf(f, "???");
}
