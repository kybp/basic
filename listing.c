#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listing.h"

void add_line_from_parent(line *parent, line *l, int line_no,
                          char *text, statement *stmt);
void stmtcopy(statement *to, statement *from);
void write_listing(line *listing, FILE *file);

void add_line(line *listing, int line_no, char *text, statement *stmt)
{
    add_line_from_parent(listing, listing, line_no, text, stmt);
}

void add_line_from_parent(line *parent, line *l, int line_no,
                          char *text, statement *stmt)
{
    if (l == NULL) {
        statement *stmt_copy = (statement *)malloc(sizeof(statement));
        char *text_copy = (char *)malloc(strlen(text) + 1);
        line *new  = (line *)malloc(sizeof(line));
        strcpy(text_copy, text);
        stmtcopy(stmt_copy, stmt);
        new->line_no = line_no;
        new->stmt    = stmt_copy;
        new->text    = text_copy;
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
        add_line_from_parent(l, l->right, line_no, text, stmt);
    } else {
        add_line_from_parent(l, l->left, line_no, text, stmt);
    }
}

void reset_listing(line *listing)
{
    if (listing == NULL) return;

    reset_listing(listing->left);
    reset_listing(listing->right);
    /* listing->text is only NULL if this is the root node, which we
     * do not allocate */
    if (listing->text) {
        free(listing->text);
        free(listing->stmt);
        free(listing);
    } else {
        listing->left = NULL;
        listing->right = NULL;
    }
}

void save_listing(line *listing, char *filename)
{
    FILE *f;

    if ((f = fopen(filename, "w")) == NULL) {
        /* handle error */
    } else {
        write_listing(listing, f);
        fclose(f);
    }
}

void stmtcopy(statement *to, statement *from)
{
    to->command = from->command;
    to->arg1    = from->arg1;
    to->arg2    = from->arg2;
}

void write_listing(line *listing, FILE *file)
{
    if (listing == NULL) return;

    write_listing(listing->left, file);
    if (listing->text)
        fprintf(file, "%d %s\n", listing->line_no, listing->text);
    write_listing(listing->right, file);
}
