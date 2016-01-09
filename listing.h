#ifndef LISTING_H
#define LISTING_H

typedef struct line {
    int line_no;
    char *text;
    struct line *left;
    struct line *right;
} line;

void add_line(line *listing, int line_no, char *text);
void save_listing(line *listing, char *filename);
void write_listing(line *listing, FILE *file);

#endif
