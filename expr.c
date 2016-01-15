#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
#include "expr.h"
#include "symtab.h"

#define MAX_LINE 1024
#define NOTHING 0

void add_expr(expr *in, expr *out, symtab *table);
int compare(expr *e1, expr *e2, int comp);
void div_expr(expr *in, expr *out, symtab *table);
void mul_expr(expr *in, expr *out, symtab *table);
void pow_expr(expr *in, expr *out, symtab *table);
void sub_expr(expr *in, expr *out, symtab *table);

void add_expr(expr *in, expr *out, symtab *table)
{
    eval_expr(in->arg1, out->arg1, table);
    eval_expr(in->arg2, out->arg2, table);
    out->op = NOTHING;
    if (out->arg1->type == INTEGER && out->arg2->type == INTEGER) {
        out->type = INTEGER;
        out->val.integer = out->arg1->val.integer + out->arg2->val.integer;
    } else if (out->arg1->type == INTEGER && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = (double)out->arg1->val.integer + out->arg2->val.real;
    } else if (out->arg1->type == REAL && out->arg2->type == INTEGER) {
        out->type = REAL;
        out->val.real = out->arg1->val.real + (double)out->arg2->val.integer;
    } else if (out->arg1->type == REAL && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = out->arg1->val.real + out->arg2->val.real;
    }
}

int compare(expr *e1, expr *e2, int comp)
{
    double x = e1->type == REAL ? e1->val.real : (double)e1->val.integer;
    double y = e2->type == REAL ? e2->val.real : (double)e2->val.integer;
    switch (comp) {
    case LT: return x <  y;
    case LE: return x <= y;
    case EQ: return x == y;
    case GE: return x >= y;
    case GT: return x >  y;
    case NE: return x != y;
    default:
        fprintf(stderr, "unrecognised comparison operator\n");
        exit(1);
    }
}

void div_expr(expr *in, expr *out, symtab *table)
{
    eval_expr(in->arg1, out->arg1, table);
    eval_expr(in->arg2, out->arg2, table);
    out->op = NOTHING;
    if (out->arg1->type == INTEGER && out->arg2->type == INTEGER) {
        out->type = INTEGER;
        out->val.integer = out->arg1->val.integer / out->arg2->val.integer;
    } else if (out->arg1->type == INTEGER && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = (double)out->arg1->val.integer / out->arg2->val.real;
    } else if (out->arg1->type == REAL && out->arg2->type == INTEGER) {
        out->type = REAL;
        out->val.real = out->arg1->val.real / (double)out->arg2->val.integer;
    } else if (out->arg1->type == REAL && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = out->arg1->val.real / out->arg2->val.real;
    }
}

void eval_expr(expr *in, expr *out, symtab *table)
{
    switch (in->type) {
    case NOTHING: break;
    case INTEGER: case REAL: case STRING:
        out->type = in->type;
        out->val  = in->val;
        return;
    case INT_VAR: {
        int n;
        if (lookup_int(in->val.string, table, &n)) {
            out->type = INTEGER;
            out->val.integer = n;
            return;
        } else {
            fprintf(stderr, "undefined integer variable '%s'\n",
                    in->val.string);
            exit(1);
        }
    } break;
    case REAL_VAR: {
        double d;
        if (lookup_real(in->val.string, table, &d)) {
            out->type = REAL;
            out->val.real = d;
            return;
        } else {
            fprintf(stderr, "undefined real variable '%s'\n",
                    in->val.string);
            exit(1);
        }
    } break;
    case STR_VAR: {
        char *s = (char *)malloc(MAX_LINE);
        if (lookup_str(in->val.string, table, &s)) {
            out->type = STRING;
            out->val.string = s;
            return;
        } else {
            fprintf(stderr, "undefined string variable '%s'\n",
                    in->val.string);
            exit(1);
        }
    } break;
    }

    switch (in->op) {
    case NOTHING:
        fprintf(stderr, "No operation for non-constant expression\n");
        exit(1);
    case ADD:
        add_expr(in, out, table);
        break;
    case SUB:
        sub_expr(in, out, table);
        break;
    case EXPT:
        pow_expr(in, out, table);
        break;
    case MUL:
        mul_expr(in, out, table);
        break;
    case DIV:
        div_expr(in, out, table);
        break;
    case LT:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, LT);
        break;
    case LE:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, LE);
        break;
    case EQ:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, EQ);
        break;
    case GE:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, GE);
        break;
    case GT:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, GT);
        break;
    case NE:
        eval_expr(in->arg1, out->arg1, table);
        eval_expr(in->arg2, out->arg2, table);
        out->type = INTEGER;
        out->val.integer = compare(out->arg1, out->arg2, NE);
        break;
    case RAND:
        eval_expr(in->arg1, out->arg1, table);
        out->type = INTEGER;
        out->val.integer = rand() % out->arg1->val.integer;
        break;
    default:
        fprintf(stderr, "unrecognised operation\n");
        exit(1);
    }
}

void mul_expr(expr *in, expr *out, symtab *table)
{
    eval_expr(in->arg1, out->arg1, table);
    eval_expr(in->arg2, out->arg2, table);
    out->op = NOTHING;
    if (out->arg1->type == INTEGER && out->arg2->type == INTEGER) {
        out->type = INTEGER;
        out->val.integer = out->arg1->val.integer * out->arg2->val.integer;
    } else if (out->arg1->type == INTEGER && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = (double)out->arg1->val.integer * out->arg2->val.real;
    } else if (out->arg1->type == REAL && out->arg2->type == INTEGER) {
        out->type = REAL;
        out->val.real = out->arg1->val.real * (double)out->arg2->val.integer;
    } else if (out->arg1->type == REAL && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = out->arg1->val.real * out->arg2->val.real;
    }
}

void pow_expr(expr *in, expr *out, symtab *table)
{
    eval_expr(in->arg1, out->arg2, table);
    eval_expr(in->arg2, out->arg2, table);
    out->op = NOTHING;
    out->type = REAL;
    out->val.real = pow(out->arg1->val.real, out->arg2->val.real);
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

expr *new_real_expr(double d)
{
    expr *e = (expr *)malloc(sizeof(expr));
    e->type = REAL;
    e->val.real = d;
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

void sub_expr(expr *in, expr *out, symtab *table)
{
    eval_expr(in->arg1, out->arg1, table);
    eval_expr(in->arg2, out->arg2, table);
    if (out->arg1->type == INTEGER && out->arg2->type == INTEGER) {
        out->type = INTEGER;
        out->val.integer = out->arg1->val.integer - out->arg2->val.integer;
    } else if (out->arg1->type == INTEGER && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = (double)out->arg1->val.integer - out->arg2->val.real;
    } else if (out->arg1->type == REAL && out->arg2->type == INTEGER) {
        out->type = REAL;
        out->val.real = out->arg1->val.real - (double)out->arg2->val.integer;
    } else if (out->arg1->type == REAL && out->arg2->type == REAL) {
        out->type = REAL;
        out->val.real = out->arg1->val.real - out->arg2->val.real;
    }
    out->op = NOTHING;
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
    case LT:
        write_expr(f, e->arg1);
        fprintf(f, " < ");
        write_expr(f, e->arg2);
        return;
    case LE:
        write_expr(f, e->arg1);
        fprintf(f, " <= ");
        write_expr(f, e->arg2);
        return;
    case EQ:
        write_expr(f, e->arg1);
        fprintf(f, " = ");
        write_expr(f, e->arg2);
        return;
    case GE:
        write_expr(f, e->arg1);
        fprintf(f, " >= ");
        write_expr(f, e->arg2);
        return;
    case NE:
        write_expr(f, e->arg1);
        fprintf(f, " <> ");
        write_expr(f, e->arg2);
        return;
    case RAND:
        fprintf(f, "RAND(");
        write_expr(f, e->arg1);
        fprintf(f, ")");
        return;
    default:
        fprintf(f, "???");
        return;
    }
}
