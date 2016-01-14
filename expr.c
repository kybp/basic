#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
#include "expr.h"
#include "symtab.h"

#define MAX_LINE 1024
#define NOTHING 0

void add_expr(expr *e, symtab *table);
void div_expr(expr *e, symtab *table);
void mul_expr(expr *e, symtab *table);
void pow_expr(expr *e, symtab *table);
void sub_expr(expr *e, symtab *table);

void add_expr(expr *e, symtab *table)
{
    eval_expr(e->arg1, table);
    eval_expr(e->arg2, table);
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

void div_expr(expr *e, symtab *table)
{
    eval_expr(e->arg1, table);
    eval_expr(e->arg2, table);
    e->op = NOTHING;
    if (e->arg1->type == INTEGER && e->arg2->type == INTEGER) {
        e->type = INTEGER;
        e->val.integer = e->arg1->val.integer / e->arg2->val.integer;
    } else if (e->arg1->type == INTEGER && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = (double)e->arg1->val.integer / e->arg2->val.real;
    } else if (e->arg1->type == REAL && e->arg2->type == INTEGER) {
        e->type = REAL;
        e->val.real = e->arg1->val.real / (double)e->arg2->val.integer;
    } else if (e->arg1->type == REAL && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = e->arg1->val.real / e->arg2->val.real;
    }
}

void eval_expr(expr *e, symtab *table)
{
    switch (e->type) {
    case NOTHING: break;
    case INTEGER: case REAL: case STRING:
        return;
    case INT_VAR: {
        int n;
        if (lookup_int(e->val.string, table, &n)) {
            e->type = INTEGER;
            e->val.integer = n;
            return;
        } else {
            fprintf(stderr, "undefined integer variable '%s'\n",
                    e->val.string);
            exit(1);
        }
    } break;
    case REAL_VAR: {
        double d;
        if (lookup_real(e->val.string, table, &d)) {
            e->type = REAL;
            e->val.real = d;
            return;
        } else {
            fprintf(stderr, "undefined real variable '%s'\n",
                    e->val.string);
            exit(1);
        }
    } break;
    case STR_VAR: {             /* segfaults */
        char *s = (char *)malloc(MAX_LINE);
        if (lookup_str(e->val.string, table, &s)) {
            e->type = STRING;
            e->val.string = s;
            return;
        } else {
            fprintf(stderr, "undefined string variable '%s'\n",
                    e->val.string);
            exit(1);
        }
    } break;
    }

    switch (e->op) {
    case NOTHING:
        fprintf(stderr, "No operation for non-constant expression\n");
        exit(1);
    case ADD:
        add_expr(e, table);
        break;
    case SUB:
        sub_expr(e, table);
        break;
    case EXPT:
        pow_expr(e, table);
        break;
    case MUL:
        mul_expr(e, table);
        break;
    case DIV:
        div_expr(e, table);
        break;
    default:
        fprintf(stderr, "unrecognised operation\n");
        exit(1);
    }
}

void mul_expr(expr *e, symtab *table)
{
    eval_expr(e->arg1, table);
    eval_expr(e->arg2, table);
    e->op = NOTHING;
    if (e->arg1->type == INTEGER && e->arg2->type == INTEGER) {
        e->type = INTEGER;
        e->val.integer = e->arg1->val.integer * e->arg2->val.integer;
    } else if (e->arg1->type == INTEGER && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = (double)e->arg1->val.integer * e->arg2->val.real;
    } else if (e->arg1->type == REAL && e->arg2->type == INTEGER) {
        e->type = REAL;
        e->val.real = e->arg1->val.real * (double)e->arg2->val.integer;
    } else if (e->arg1->type == REAL && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = e->arg1->val.real * e->arg2->val.real;
    }
}

void pow_expr(expr *e, symtab *table)
{
    eval_expr(e->arg1, table);
    eval_expr(e->arg2, table);
    e->op = NOTHING;
    e->type = REAL;
    e->val.real = pow(e->arg1->val.real, e->arg2->val.real);
}

expr *new_expr(expr *arg1, int op, expr *arg2)
{
    expr *e = (expr *)malloc(sizeof(expr));
    e->type = NOTHING;
    e->op   = op;
    e->arg1 = arg1;
    e->arg2 = arg2;
    return e;
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

void sub_expr(expr *e, symtab *table)
{
    eval_expr(e->arg1, table);
    eval_expr(e->arg2, table);
    e->op = NOTHING;
    if (e->arg1->type == INTEGER && e->arg2->type == INTEGER) {
        e->type = INTEGER;
        e->val.integer = e->arg1->val.integer - e->arg2->val.integer;
    } else if (e->arg1->type == INTEGER && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = (double)e->arg1->val.integer - e->arg2->val.real;
    } else if (e->arg1->type == REAL && e->arg2->type == INTEGER) {
        e->type = REAL;
        e->val.real = e->arg1->val.real - (double)e->arg2->val.integer;
    } else if (e->arg1->type == REAL && e->arg2->type == REAL) {
        e->type = REAL;
        e->val.real = e->arg1->val.real - e->arg2->val.real;
    }
}

void write_expr(FILE *f, expr *e)
{
    switch (e->type) {
    case INTEGER:
        fprintf(f, "%d", e->val.integer);
        return;
    case REAL:
        fprintf(f, "%f", e->val.real);
        return;
    case STRING:
        fprintf(f, "\"%s\"", e->val.string);
        return;
    case INT_VAR: case REAL_VAR: case STR_VAR:
        fprintf(f, "%s", e->val.string);
        return;
    }

    switch (e->op) {
    case ADD:
        write_expr(f, e->arg1);
        fprintf(f, " + ");
        write_expr(f, e->arg2);
        return;
    case SUB:
        write_expr(f, e->arg1);
        fprintf(f, " - ");
        write_expr(f, e->arg2);
        return;
    case MUL:
        write_expr(f, e->arg1);
        fprintf(f, " * ");
        write_expr(f, e->arg2);
        return;
    case DIV:
        write_expr(f, e->arg1);
        fprintf(f, " / ");
        write_expr(f, e->arg2);
        return;
    case EXPT:
        write_expr(f, e->arg1);
        fprintf(f, " ** ");
        write_expr(f, e->arg2);
        return;
    default:
        fprintf(f, "???");
        return;
    }
}
