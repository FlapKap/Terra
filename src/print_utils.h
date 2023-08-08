#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include "operators.h"
#include "number.h"

void print_expression_instruction(ExpressionInstruction instr);
void print_instruction(const Instruction *instr);
void print_instruction_array(const Instruction* instruction, size_t size);
void print_number_value(Number val);
void print_number_value_and_ucase(Number number);
void print_stack(const Stack *stack);
void print_env(const Env *env);
void print_expression(const Expression *expression);
void print_map(const Map *map);
void print_filter(const Filter *filter);
void print_window_aggregation_type(WindowAggregationType type);
void print_window_size_type(WindowSizeType type);
void print_window(Window *window);
void print_operation(const Operation *operation);
void print_query(const Query *query);
void print_message(const Message *message);
void print_query_response(const QueryResponse *response);
void print_output_message(const OutputMessage *message);


#endif // PRINT_UTILS_H