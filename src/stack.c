#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "operators.h"
#include "number.h"

Stack *init_stack(void)
{
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    stack->stack = (Number*) calloc(10, sizeof(Number));
    stack->size = 10;
    stack->top = -1;
    return stack;
}

void push(Stack *stack, Number val)
{
    if (stack->top == stack->size - 1)
    {
        resize_stack(stack);
    }
    stack->top++;
    stack->stack[stack->top] = val;
}

Number pop(Stack *stack)
{
    if(stack->top == -1) {
        printf("Stack is empty\n");
        return (Number){{CONST}, 2};
    }
    return stack->stack[stack->top--];
}

// TODO: Optimal algorithm for stack resizing?
void resize_stack(Stack *stack)
{
    stack->size += 10;
    stack->stack = (Number*) realloc(stack->stack, stack->size * sizeof(int));
}

void print_stack(Stack *stack)
{
    //TODO: make the print stack work with Numbers
    printf("\ntop: %d\n", stack->top);
    // printf("stack size: %d\n", stack->size);
    printf("stack: [");
    for (int i = 0; i < stack->top+1; i++)
    {
        printNumberValue(stack->stack[i]);
        if (i != stack->top)
        {
            printf(", ");
        }
    }
    printf("]\n\n");
}
