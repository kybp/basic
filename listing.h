#ifndef LISTING_H
#define LISTING_H

union argument {
    int integer;
    char *string;
};

typedef struct statement {
    int command;
    union argument arg1;
    union argument arg2;
} statement;

typedef struct line {
    int line_no;
    statement *stmt;
    struct line *left;
    struct line *right;
} line;

void add_line(line *listing, int line_no, statement *stmt);
void reset_listing(line *listing);
void save_listing(line *listing, char *filename);
void write_listing(line *listing, FILE *file);

#endif
