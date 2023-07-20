#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include "operators.h"
#include "number.h"

void printExpressionInstruction(ExpressionInstruction instr);
void printInstruction(const Instruction *instr);
void printInstructionArray(const Instruction* instruction, size_t size);
void printNumberValue(Number val);
void printNumberValueAndUcase(Number number);
void printStack(const Stack *stack);
void printEnv(const Env *env);
void printExpression(const Expression *expression);
void printMap(const Map *map);
void printFilter(const Filter *filter);
void printWindowAggregationType(WindowAggregationType type);
void printWindowSizeType(WindowSizeType type);
void printWindow(Window *window);
void printOperation(const Operation *operation);
void printQuery(const Query *query);
void printMessage(const Message *message);
void printQueryResponse(const QueryResponse *response);
void printOutputMessage(const OutputMessage *message);

#endif // PRINT_UTILS_H