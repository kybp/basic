#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.tab.h"
#include "listing.h"
#include "stack.h"

void add_line_from_parent(line *parent, line *l, int line_no, statement *stmt);
line *find_line(line *listing, int line_no);
line *find_min(line *listing);
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
        new->parent  = parent;
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

void eval_listing(line *listing)
{
    stack st;
    line *root = listing;
    init_stack(&st, 16);

    listing = find_min(listing->right);
    if (listing->stmt == NULL) {  /* no lines in listing */
        return;
    }

    while (listing) {
        int next = eval_stmt(listing->stmt, &st);

        switch (next) {
        case 0: return;
        case NEXT_LINE:
            if (listing->right) {
                listing = find_min(listing->right);
            } else if (listing->parent->line_no > listing->line_no) {
                listing = listing->parent;
            } else {
                return;
            }
            break;
        default:
            listing = find_line(root, next);
            if (listing == NULL) {
                /* handle error; no such line */
            }
        }
    }
}

/*
 * eval_stmt() returns the next line number to execute at. Return
 * NEXT_LINE to progress to the next line, or 0 to stop execution
 */
int eval_stmt(statement *stmt, stack *st) {
    /* If the stack is NULL that means we're running interactively, so
     * any commands that modify the stack should check for NULL and
     * refuse to run */
    switch(stmt->command) {
    case GOSUB:
        push(st, stmt->arg1.integer);
    case GOTO:
        return stmt->arg1.integer;
    case PRINT:
        switch (stmt->type) {
        case STRING:
            printf("%s\n", stmt->arg1.string);
            break;
        case INTEGER:
            printf("%d\n", stmt->arg1.integer);
            break;
        case REAL:
            printf("%f\n", stmt->arg1.real);
            break;
        case NOTHING:
            printf("\n");
            break;
        }
        break;
    case RETURN:
        if (stack_is_empty(st)) {
            return NOTHING;
        } else {
            return pop(st);
        }
        break;
    }
    return NEXT_LINE;
}

line *find_line(line *listing, int line_no)
{
    if (listing == NULL || listing->line_no == line_no) {
        return listing;
    } else if (line_no < listing->line_no) {
        return find_line(listing->left, line_no);
    } else {
        return find_line(listing->right, line_no);
    }
}

line *find_min(line *listing)
{
    line *min = listing;
    while (listing->left) min = listing->left;
    return min;
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
    to->type    = from->type;
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
    case PRINT:
        switch (stmt->type) {
        case STRING:
            fprintf(f, "PRINT \"%s\"\n", stmt->arg1.string);
            break;
        case INTEGER:
            fprintf(f, "PRINT %d\n", stmt->arg1.integer);
            break;
        case REAL:
            fprintf(f, "PRINT %f\n", stmt->arg1.real);
            break;
        case NOTHING:
            fprintf(f, "PRINT\n");
            break;
        }
        break;
    case RETURN:
        fprintf(f, "RETURN\n");
        break;
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
