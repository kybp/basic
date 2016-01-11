#ifndef LISTING_H
#define LISTING_H
#include "expr.h"
#include "stack.h"
#include "symtab.h"

#define NOTHING 0
#define NEXT_LINE -1

typedef struct statement {
    int command;
    expr *arg1;
    expr *arg2;
} statement;

typedef struct line {
    int line_no;
    statement *stmt;
    struct line *left;
    struct line *right;
    struct line *parent;
} line;

void add_line(line *listing, int line_no, statement *stmt);
void eval_listing(line *listing, symtab *table);
int  eval_stmt(statement *stmt, stack *st, symtab *table);
void reset_listing(line *listing);
void save_listing(line *listing, char *filename);
void write_listing(line *listing, FILE *file);

#endif
