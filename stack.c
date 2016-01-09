#include <stdlib.h>
#include "stack.h"

void init_stack(stack *s, int size)
{
    int *contents = (int *)malloc(sizeof(int) * size);

    s->contents = contents;
    s->size     = size;
    s->top      = -1;
}

void reset_stack(stack *s)
{
    s->top = -1;
}

int stack_is_empty(stack *s)
{
    return s->top == -1;
}

int stack_is_full(stack *s)
{
    return s->top == s->size - 1;
}

void push(stack *s, int n)
{
    if (!stack_is_full(s)) {
        s->contents[++s->top] = n;
    }
}

int pop(stack *s)
{
    return s->contents[s->top--];
}
