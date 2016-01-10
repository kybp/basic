#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void new_symtab(identifier *id, symtab *table);

void defvar(identifier *id, symtab *table)
{
    int comp;

    if (table->val == NULL) {
        new_symtab(id, table);
        return;
    }

    comp = strcmp(id->name, table->val->name);

    if (comp == 0) {
        table->val = (identifier *)malloc(sizeof(identifier));
        table->val->name = (char *)malloc(strlen(id->name) + 1);
        strcpy(table->val->name, id->name);
        table->val->type = id->type;
        table->val->value = id->value;
    } else if (comp < 0) {
        if (table->left == NULL) {
            table->left = (symtab *)malloc(sizeof(symtab));
            new_symtab(id, table->left);
        } else {
            defvar(id, table->left);
        }
    } else {
        if (table->right == NULL) {
            table->right = (symtab *)malloc(sizeof(symtab));
            new_symtab(id, table->right);
        } else {
            defvar(id, table->right);
        }
    }
}

int lookup_int(char *name, symtab *table, int *n)
{
    int comp;

    if (table == NULL) return 0;

    comp = strcmp(name, table->val->name);
    if (comp == 0) {
        *n = table->val->value.integer;
        return 1;
    } else if (comp < 0) {
        return lookup_int(name, table->left, n);
    } else {
        return lookup_int(name, table->right, n);
    }
}

int lookup_real(char *name, symtab *table, double *n)
{
    int comp;

    if (table == NULL) return 0;

    comp = strcmp(name, table->val->name);
    if (comp == 0) {
        *n = table->val->value.real;
        return 1;
    } else if (comp < 0) {
        return lookup_real(name, table->left, n);
    } else {
        return lookup_real(name, table->right, n);
    }
}

int lookup_str(char *name, symtab *table, char **s)
{
    int comp;

    if (table == NULL) return 0;

    comp = strcmp(name, table->val->name);
    if (comp == 0) {
        *s = table->val->value.string;
        return 1;
    } else if (comp < 0) {
        return lookup_str(name, table->left, s);
    } else {
        return lookup_str(name, table->right, s);
    }
}

void new_symtab(identifier *id, symtab *table)
{
    table->val = (identifier *)malloc(sizeof(identifier));
    table->val->name = (char *)malloc(strlen(id->name) + 1);
    strcpy(table->val->name, id->name);
    table->val->type = id->type;
    table->val->value = id->value;
}
