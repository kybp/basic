#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include "symtab.h"

typedef struct expr {
    int op;
    int type;
    union {
        int integer;
        double real;
        char *string;
    } val;
    struct expr *arg1;
    struct expr *arg2;
} expr;

void eval_expr(expr *in, expr *out, symtab *table);
expr *new_expr(expr *arg1, int op, expr *arg2);
expr *new_int_expr(int n);
expr *new_real_expr(double d);
expr *new_str_expr(char *s);
expr *new_var_expr(int type, char *name);
void write_expr(FILE *f, expr *e);

#endif
