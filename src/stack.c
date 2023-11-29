#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "operators.h"
#include "number.h"

#define ENABLE_DEBUG  0
#include <debug.h>


void init_stack(Number* memory, size_t size, Stack* out)
{
    
    out->stack_memory = memory;
    out->size = size;
    out->top = -1;
}

void push(Stack *stack, Number val)
{
    DEBUG("push %d\n to stack with size before: %d. top: %d\n", val.type._int, stack->size, stack->top);
    // if (stack->top == stack->size - 1)
    // {
    //     resize_stack(stack);
    // }
    stack->top++;
    stack->stack_memory[stack->top] = val;
}

Number pop(Stack *stack)
{
    DEBUG("pop from stack with size before: %d. top: %d\n", stack->size, stack->top);
    if(stack->top == -1) {
        printf("Stack is empty\n");
        return (Number){NUMBER_UINT32, 2};
    }
    return stack->stack_memory[stack->top--];
}

// // TODO: change stack to be static size
// void resize_stack(Stack *stack)
// {
//     stack->size += 10;
//     stack->stack_memory = (Number*) realloc(stack->stack, stack->size * sizeof(int));
// }

