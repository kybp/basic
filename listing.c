#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
#include "listing.h"

#define MAX_LINE 1024

void add_line_from_parent(line *parent, line *l, int line_no, statement *stmt);
void stmtcopy(statement *to, statement *from);
void write_listing(line *listing, FILE *file);

void add_line(line *listing, int line_no, statement *stmt)
{
    add_line_from_parent(listing, listing, line_no, stmt);
}

void add_line_from_parent(line *parent, line *l, int line_no, statement *stmt)
{
    if (l == NULL) {
        statement *stmt_copy = (statement *)malloc(sizeof(statement));
        line *new  = (line *)malloc(sizeof(line));
        stmtcopy(stmt_copy, stmt);
        new->line_no = line_no;
        new->stmt    = stmt_copy;
        new->left    = NULL;
        new->right   = NULL;
        l = new;
        if (parent && parent->line_no > line_no) {
            parent->left = new;
        } else if (parent) {
            parent->right = new;
        }
    } else if (l->line_no == line_no) {
        statement *copy = (statement *)malloc(sizeof(statement));
        stmtcopy(copy, stmt);
        free(l->stmt);
        l->stmt = copy;
    } else if (l->line_no < line_no) {
        add_line_from_parent(l, l->right, line_no, stmt);
    } else {
        add_line_from_parent(l, l->left, line_no, stmt);
    }
}

void reset_listing(line *listing)
{
    if (listing == NULL) return;

    reset_listing(listing->left);
    reset_listing(listing->right);
    /* listing->stmt is only NULL if this is the root node, which we
     * do not allocate */
    if (listing->stmt) {
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

void write_stmt(statement *stmt, FILE *f) {
    switch (stmt->command) {
    case GOSUB:
        fprintf(f, "GOSUB %d\n", stmt->arg1.integer);
        break;
    case GOTO:
        fprintf(f, "GOTO %d\n", stmt->arg1.integer);
        break;
    case LIST:
        fprintf(f, "LIST\n");
        break;
    case LOAD:
        fprintf(f, "LOAD\n");
        break;
    case NEW:
        fprintf(f, "NEW\n");
        break;
    case RETURN:
        fprintf(f, "RETURN\n");
        break;
    case SAVE:
        fprintf(f, "SAVE\n");
        break;
    default:
        fprintf(f, "???\n");
    }
}

void write_listing(line *listing, FILE *file)
{
    if (listing == NULL) return;

    write_listing(listing->left, file);
    if (listing->stmt) {
        fprintf(file, "%d ", listing->line_no);
        write_stmt(listing->stmt, file);
    }
    write_listing(listing->right, file);
}
