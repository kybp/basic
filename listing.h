#ifndef LISTING_H
#define LISTING_H
#include "stack.h"

#define NOTHING 0

union argument {
    int integer;
    double real;
    char *string;
};

typedef struct statement {
    int command;
    int type;
    union argument arg1;
    union argument arg2;
} statement;

typedef struct line {
    int line_no;
    statement *stmt;
    struct line *left;
    struct line *right;
    struct line *parent;
} line;

void add_line(line *listing, int line_no, statement *stmt);
void eval_listing(line *listing);
int  eval_stmt(statement *stmt, stack *st);
void reset_listing(line *listing);
void save_listing(line *listing, char *filename);
void write_listing(line *listing, FILE *file);

#endif
