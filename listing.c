#include <stdlib.h>
#include <string.h>
#include "listing.h"

void add_line_from_parent(line *parent, line *l, int line_no, char *text);

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
