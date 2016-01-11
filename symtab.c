#include <stdlib.h>
#include <string.h>
#include "symtab.h"

identifier *lookup_val(char *name, symtab *table);
void new_symtab(char *name, int type, void *data, symtab *table);

void defvar(char *name, int type, void *data, symtab *table)
{
    int comp;

    if (table->val == NULL) {
        new_symtab(name, type, data, table);
        return;
    }

    comp = strcmp(name, table->val->name);

    if (comp == 0) {
        table->val = (identifier *)malloc(sizeof(identifier));
        table->val->name = (char *)malloc(strlen(name) + 1);
        strcpy(table->val->name, name);
        table->val->type = type;
        table->val->data = data;
    } else if (comp < 0) {
        if (table->left == NULL) {
            table->left = (symtab *)malloc(sizeof(symtab));
            new_symtab(name, type, data, table->left);
        } else {
            defvar(name, type, data, table->left);
        }
    } else {
        if (table->right == NULL) {
            table->right = (symtab *)malloc(sizeof(symtab));
            new_symtab(name, type, data, table->right);
        } else {
            defvar(name, type, data, table->right);
        }
    }
}

int lookup_int(char *name, symtab *table, int *n)
{
    identifier *id = lookup_val(name, table);
    if (id == NULL) {
        return 0;
    } else {
        *n = *(int *)id->data;
        return 1;
    }
}

int lookup_real(char *name, symtab *table, double *n)
{
    identifier *id = lookup_val(name, table);
    if (id == NULL) {
        return 0;
    } else {
        *n = *(double *)id->data;
        return 1;
    }
}

int lookup_str(char *name, symtab *table, char **s)
{
    identifier *id = lookup_val(name, table);
    if (id == NULL) {
        return 0;
    } else {
        *s = *(char *)id->data;
        return 1;
    }
}

identifier *lookup_val(char *name, symtab *table)
{
    int comp;

    if (table == NULL) return NULL;

    comp = strcmp(name, table->val->name);
    if (comp == 0) {
        return table->val;
    } else if (comp < 0) {
        return lookup_val(name, table->left);
    } else {
        return lookup_val(name, table->right);
    }
}

void new_symtab(char *name, int type, void *data, symtab *table)
{
    table->val = (identifier *)malloc(sizeof(identifier));
    table->val->name = (char *)malloc(strlen(name) + 1);
    strcpy(table->val->name, name);
    table->val->type = type;
    table->val->data = data;
}
