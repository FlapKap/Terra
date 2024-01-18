#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdlib.h>
#include <stdint.h>
#include "number.h"
#include "stack.h"
#include "environment.h"
#include <terraprotocol.pb.h>

// struct Operator {
//   int (*call) (int *func);
// };

// typedef enum _expression_instruction {
//   CONST = 0,
//   VAR = 1,
//   AND = 2,
//   OR = 3,
//   NOT = 4,
//   LT = 5,
//   GT = 6,
//   EQ = 7,
//   ADD = 8,
//   SUB = 9,
//   MUL = 10,
//   DIV = 11,
//   MOD = 12,
//   LOG = 13,
//   POW = 14,
//   SQRT = 15,
//   EXP = 16,
//   CEIL = 17,
//   FLOOR = 18,
//   ROUND = 19,
//   ABS = 20,
//   LTEQ = 21,
//   GTEQ = 22,
// } ExpressionInstruction;

// typedef EndDeviceProtocol_ExpressionInstructions ExpressionInstruction;

typedef enum _instruction_union_case_t {
  INSTRUCTION_INSTRUCTION,
  INSTRUCTION_UINT32,
  INSTRUCTION_INT,
  INSTRUCTION_FLOAT,
  INSTRUCTION_DOUBLE
} instruction_union_case_t;

// typedef struct _Instruction
// {
//   union {
//     ExpressionInstruction _instruction; // case 0
//     uint32_t _uint32;                   // case 1
//     int _int;                           // case 2
//     float _float;                       // case 3
//     double _double;                     // case 4
//   } data;
//   instruction_union_case_t unionCase;
// } Instruction;

typedef struct _Expression
{
    TerraProtocol_Expression* program;
    int pc;
    Env *env;
    Stack *stack;
} Expression;


// typedef struct _Map
// {
//   Expression *expression;
//   int attribute;
// } Map;

// typedef struct _Filter
// {
//   Expression *predicate;
// } Filter;

// typedef enum __attribute__((packed)) _WindowAggregationType {
//   MIN,
//   MAX,
//   SUM,
//   AVG,
//   COUNT
// } WindowAggregationType;

// typedef enum __attribute__((packed)) _WindowSizeType {
//   TIMEBASED,
//   COUNTBASED
// } WindowSizeType;

// typedef struct _Window
// {
//   int size;
//   WindowSizeType sizeType;
//   WindowAggregationType aggregationType;
//   int startAttribute;
//   int endAttribute;
//   int resultAttribute;
//   int readAttribute;
// } Window;

// typedef struct _Operation
// {
//   union {
//     Map *map;
//     Filter *filter;
//     Window *window;
//   } operation;
//   int unionCase;
// } Operation;

// typedef struct _Query
// {
//   Operation *operations;
//   int amount;
// } Query;

// typedef struct _Message
// {
//   Query *queries;
//   int amount;
// } Message;

// typedef struct _QueryResponse
// {
//   int id;
//   Instruction *response;
//   int amount;
// } QueryResponse;

// typedef struct _OutputMessage
// {
//   QueryResponse* responses;
//   int amount;
// } OutputMessage;

bool bin_op(const Number* n1, const Number* n2, const TerraProtocol_ExpressionInstructions op, Number* result);
bool un_op(const Number* n1, const TerraProtocol_ExpressionInstructions op, Number* result);

void copy_number_to_instruction(const Number* src, TerraProtocol_Data* dest);
void copy_instruction_to_number(const TerraProtocol_Data* src, Number* dest);

#endif
