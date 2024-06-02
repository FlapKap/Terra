#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "number.h"

typedef struct _Stack
{
    int top;
    Number stack_memory[RUNTIME_STACK_MEMORY];
} Stack;

void stack_push(Stack* stack, Number val);
Number stack_pop(Stack* stack);
void stack_clear_stack(Stack* stack);

#endif /* STACK_H */
