#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct identifier {
    char *name;
    int type;
    union {
        int integer;
        double real;
        char *string;
    } value;
} identifier;

typedef struct symtab {
    identifier *val;
    struct symtab *left;
    struct symtab *right;
    struct symtab *parent;
} symtab;

void defvar(identifier *id, symtab *table);
int lookup_int(char *name, symtab *table, int *n);
int lookup_real(char *name, symtab *table, double *n);
int lookup_str(char *name, symtab *table, char **s);

#endif
