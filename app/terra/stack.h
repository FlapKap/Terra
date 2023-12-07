#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "number.h"

typedef struct _Stack
{
    int top;
    int size;
    Number* stack_memory;
} Stack;

void stack_init_stack(Number* memory, size_t memory_size, Stack* out);
void stack_push(Stack* stack, Number val);
Number stack_pop(Stack* stack);
void stack_clear_stack(Stack* stack);
//void resize_stack(Stack *stack);

#endif /* STACK_H */
