#ifndef STACK_H
#define STACK_H

typedef struct stack {
    int *contents;
    int top;
    int size;
} stack;

void init_stack(stack *s, int size);
void reset_stack(stack *s);
int stack_is_empty(stack *s);
int stack_is_full(stack *s);
void push(stack *s, int n);
int pop(stack *s);

#endif
