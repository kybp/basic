#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listing.h"

void add_line_from_parent(line *parent, line *l, int line_no, char *text);
void write_listing(line *listing, FILE *file);

void add_line(line *listing, int line_no, char *text)
{
    add_line_from_parent(listing, listing, line_no, text);
}

void add_line_from_parent(line *parent, line *l, int line_no, char *text)
{
    if (l == NULL) {
        char *copy = (char *)malloc(strlen(text) + 1);
        strcpy(copy, text);
        line *new = (line *)malloc(sizeof(line));
        new->line_no = line_no;
        new->text    = copy;
        new->left    = NULL;
        new->right   = NULL;
        l = new;
        if (parent && parent->line_no > line_no) {
            parent->left = new;
        } else if (parent) {
            parent->right = new;
        }
    } else if (l->line_no == line_no) {
        char *copy = (char *)malloc(strlen(text) + 1);
        strcpy(copy, text);
        free(l->text);
        l->text = copy;
    } else if (l->line_no < line_no) {
        add_line_from_parent(l, l->right, line_no, text);
    } else {
        add_line_from_parent(l, l->left, line_no, text);
    }
}

void save_listing(line *listing, char *filename)
{
    FILE *f;

    if ((f = fopen(filename, "w")) == NULL) {
        /* handle error */
    } else {
        write_listing(listing, f);
    }
}

void write_listing(line *listing, FILE *file)
{
    if (listing == NULL) return;

    write_listing(listing->left, file);
    if (listing->text)
        fprintf(file, "%d %s\n", listing->line_no, listing->text);
    write_listing(listing->right, file);
}
