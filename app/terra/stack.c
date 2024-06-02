#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "operators.h"
#include "number.h"

#define ENABLE_DEBUG  0
#include <debug.h>


void stack_push(Stack *stack, Number val)
{
    DEBUG("push %d\n to stack with top: %d\n", val.type._int, stack->top);
    stack->top++;
    stack->stack_memory[stack->top] = val;
}

Number stack_pop(Stack *stack)
{
    DEBUG("pop from stack with top: %d\n", stack->top);
    if(stack->top == -1) {
        printf("[stack.c] ERROR: Stack is empty\n");
        return (Number){{NUMBER_UINT32}, 2};
    }
    return stack->stack_memory[stack->top--];
}

void stack_clear_stack(Stack *stack)
{
    for (int i = 0; i < RUNTIME_STACK_MEMORY; i++)
    {
        stack->stack_memory[i].type._int = 0;
        stack->stack_memory[i].unionCase = NUMBER_UINT32;
    }
    stack->top = -1;
}

