#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "operators.h"
#include "number.h"

#define ENABLE_DEBUG  0
#include <debug.h>


void stack_init_stack(Number* memory, size_t size, Stack* out)
{
    
    out->stack_memory = memory;
    out->size = size;
    out->top = -1;
    stack_clear_stack(out);
}

void stack_push(Stack *stack, Number val)
{
    DEBUG("push %d\n to stack with size before: %d. top: %d\n", val.type._int, stack->size, stack->top);
    // if (stack->top == stack->size - 1)
    // {
    //     resize_stack(stack);
    // }
    stack->top++;
    stack->stack_memory[stack->top] = val;
}

Number stack_pop(Stack *stack)
{
    DEBUG("pop from stack with size before: %d. top: %d\n", stack->size, stack->top);
    if(stack->top == -1) {
        printf("[stack.c] ERROR: Stack is empty\n");
        return (Number){{NUMBER_UINT32}, 2};
    }
    return stack->stack_memory[stack->top--];
}

void stack_clear_stack(Stack *stack)
{
    for (int i = 0; i < stack->size; i++)
    {
        stack->stack_memory[i].type._int = 0;
        stack->stack_memory[i].unionCase = NUMBER_UINT32;
    }
    stack->top = -1;
}

// // TODO: change stack to be static size
// void resize_stack(Stack *stack)
// {
//     stack->size += 10;
//     stack->stack_memory = (Number*) realloc(stack->stack, stack->size * sizeof(int));
// }

