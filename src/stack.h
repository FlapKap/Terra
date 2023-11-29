#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "number.h"
#include "operators.h"

typedef struct _Stack
{
    int top;
    int size;
    Number *stack_memory;
} Stack;

void init_stack(Number* memory, size_t memory_size, Stack* out);
void push(Stack *stack, Number val);
Number pop(Stack *stack);
//void resize_stack(Stack *stack);

#endif /* STACK_H */
