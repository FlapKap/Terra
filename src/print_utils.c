#include "print_utils.h"
#include <stdio.h>
#include <inttypes.h>

// #lizard forgives
void printExpressionInstruction(ExpressionInstruction instr)
{
    switch (instr)
    {
    case CONST:
        printf("CONST");
        break;
    case VAR:
        printf("VAR");
        break;
    case AND:
        printf("AND");
        break;
    case OR:
        printf("OR");
        break;
    case NOT:
        printf("NOT");
        break;
    case LT:
        printf("LT");
        break;
    case GT:
        printf("GT");
        break;
    case EQ:
        printf("EQ");
        break;
    case ADD:
        printf("ADD");
        break;
    case SUB:
        printf("SUB");
        break;
    case MUL:
        printf("MUL");
        break;
    case DIV:
        printf("DIV");
        break;
    case MOD:
        printf("MOD");
        break;
    case LOG:
        printf("LOG");
        break;
    case POW:
        printf("POW");
        break;
    case SQRT:
        printf("SQRT");
        break;
    case EXP:
        printf("EXP");
        break;
    case CEIL:
        printf("CEIL");
        break;
    case FLOOR:
        printf("FLOOR");
        break;
    case ROUND:
        printf("ROUND");
        break;
    case ABS:
        printf("ABS");
        break;
    case LTEQ:
        printf("LTEQ");
        break;
    case GTEQ:
        printf("GTEQ");
        break;
    default:
        printf("Invalid ExpressionInstruction");
        break;
    }
}

void printInstruction(const Instruction *instruction)
{
    switch (instruction->unionCase)
    {
    case 0:
        printExpressionInstruction(instruction->data._instruction);
        printf(" (ExpressionInstruction)");
        break;
    case 1:
        printf("%" PRIu32 " (uint32_t)", instruction->data._uint32);
        break;
    case 2:
        printf("%d (int)", instruction->data._int);
        break;
    case 3:
        printf("%f (float)", instruction->data._float);
        break;
    case 4:
        printf("%lf (double)", instruction->data._double);
        break;
    default:
        printf("Invalid union case: %d", instruction->unionCase);
        break;
    }
}

void printInstructionArray(const Instruction *instruction, size_t size)
{
    printf("[");
    for (size_t i = 0; i < size; i++)
    {
        printInstruction(&instruction[i]);
        printf(", ");
    }
    printf("]");
}

void printNumberValue(Number val)
{
    switch (val.unionCase)
    {
    case 1:

        printf("%" PRIu32, val.type._uint32);
        break;
    case 2:
        printf("%d", val.type._int);
        break;
    case 3:
        printf("%f", val.type._float);
        break;
    case 4:
        printf("%lf", val.type._double);
        break;
    default:
        printf("Error: Invalid unionCase: %" PRIu8, val.unionCase);
        break;
    }
}

void printNumberValueAndUcase(Number number)
{
    switch (number.unionCase)
    {
    case 1:
        printf("%" PRIu32 " (uint)", number.type._uint32);
        break;
    case 2:
        printf("%d (int)", number.type._int);
        break;
    case 3:
        printf("%f (float)", number.type._float);
        break;
    case 4:
        printf("%lf (double)", number.type._double);
        break;
    default:
        printf("Error: Invalid unionCase: %" PRIu8, number.unionCase);
        break;
    }
}

void printStack(const Stack *stack)
{
    printf("Stack (top: %d, size: %d):\n", stack->top, stack->size);
    printf("Elements:\n");
    for (int i = stack->top; i >= 0; i--)
    {
        printf("  %d. ", stack->top - i + 1);
        printNumberValueAndUcase(stack->stack[i]);
        printf("\n");
    }
}

void printEnv(const Env *env)
{
    printf("Env (size: %d):\n", env->size);
    printf("Array:\n");
    for (int i = 0; i < env->size; i++)
    {
        printf("  %d. ", i + 1);
        printNumberValueAndUcase(env->memory[i]);
        printf("\n");
    }

    printf("Stack:\n");
    printStack(env->stack);
}

void printExpression(const Expression *expression)
{
    printf("Expression (p_size: %d, pc: %d):\n", expression->p_size, expression->pc);
    printf("Program:\n");
    printInstructionArray(expression->program, expression->p_size);
    printf("\n");
    printf("Environment:\n");
    printEnv(expression->env);

    printf("Stack:\n");
    printStack(expression->stack);
}

void printMap(const Map *map)
{
    printf("Map (attribute: %d):\n", map->attribute);
    printf("Expression:\n");
    printExpression(map->expression);
}

void printFilter(const Filter *filter)
{
    printf("Filter:\n");
    printf("Predicate:\n");
    printExpression(filter->predicate);
}

void printWindowAggregationType(WindowAggregationType type)
{
    switch (type)
    {
    case MIN:
        printf("MIN");
        break;
    case MAX:
        printf("MAX");
        break;
    case SUM:
        printf("SUM");
        break;
    case AVG:
        printf("AVG");
        break;
    case COUNT:
        printf("COUNT");
        break;
    default:
        printf("Unknown WindowAggregationType");
        break;
    }
}

void printWindowSizeType(WindowSizeType type)
{
    switch (type)
    {
    case TIMEBASED:
        printf("TIMEBASED");
        break;
    case COUNTBASED:
        printf("COUNTBASED");
        break;
    default:
        printf("Unknown WindowSizeType");
        break;
    }
}

void printWindow(Window *window)
{
    printf("Window:\n");
    printf("size: %d\n", window->size);

    printf("sizeType: ");
    printWindowSizeType(window->sizeType);
    printf("\n");
    printf("aggregationType: ");
    printWindowAggregationType(window->aggregationType);
    printf("\n");

    printf("startAttribute: %d\n", window->startAttribute);
    printf("endAttribute: %d\n", window->endAttribute);
    printf("resultAttribute: %d\n", window->resultAttribute);
    printf("readAttribute: %d\n", window->readAttribute);
}

void printOperation(const Operation *operation)
{
    switch (operation->unionCase)
    {
    case 1:
        printf("Map operation:\n");
        printMap(operation->operation.map);
        break;
    case 2:
        printf("Filter operation:\n");
        printFilter(operation->operation.filter);
        break;
    case 3:
        printf("Window operation:\n");
        printWindow(operation->operation.window);
        break;
    default:
        printf("Unknown operation\n");
        break;
    }
}

void printQuery(const Query *query)
{
    printf("Query (amount: %d):\n", query->amount);
    for (int i = 0; i < query->amount; i++)
    {
        printf("Operation %d:\n", i + 1);
        printOperation(&query->operations[i]);
    }
}

void printMessage(const Message *message)
{
    printf("Message (amount of queries: %d):\n", message->amount);
    for (int i = 0; i < message->amount; i++)
    {
        printf("Query %d:\n", i + 1);
        printQuery(&message->queries[i]);
    }
}

void printQueryResponse(const QueryResponse *response)
{
    printf("Query Response (id: %d, amount of instructions: %d):\n", response->id, response->amount);
    for (int i = 0; i < response->amount; i++)
    {
        printf("Instruction %d:\n", i + 1);
        printInstruction(&response->response[i]);
    }
}

void printOutputMessage(const OutputMessage *message)
{
    printf("Output Message (amount of responses: %d):\n", message->amount);
    for (int i = 0; i < message->amount; i++)
    {
        printf("Response %d:\n", i + 1);
        printQueryResponse(&message->responses[i]);
    }
}
