#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdlib.h>
#include <stdint.h>
#include "number.h"

// struct Operator {
//   int (*call) (int *func);
// };


typedef struct _Stack
{
    int top;
    int size;
    Number *stack;
} Stack;

typedef struct _Env
{
    Number *memory;
    int size;
    Stack *stack;
} Env;

typedef enum {
  CONST = 0,
  VAR = 1,
  AND = 2,
  OR = 3,
  NOT = 4,
  LT = 5,
  GT = 6,
  EQ = 7,
  ADD = 8,
  SUB = 9,
  MUL = 10,
  DIV = 11,
  MOD = 12,
  LOG = 13,
  POW = 14,
  SQRT = 15,
  EXP = 16,
  CEIL = 17,
  FLOOR = 18,
  ROUND = 19,
  ABS = 20,
  LTEQ = 21,
  GTEQ = 22,
} ExpressionInstruction;

typedef struct _Instruction
{
  union {
    ExpressionInstruction _instruction; // case 0
    uint32_t _uint32;                  // case 1
    int _int;                          // case 2
    float _float;                      // case 3
    double _double;                    // case 4
  } data;
  int unionCase;
} Instruction;

typedef struct _Expression
{
    Instruction *program;
    int p_size;
    int pc;
    Env *env;
    Stack *stack;
} Expression;


typedef struct _Map
{
  Expression *expression;
  int attribute;
} Map;

typedef struct _Filter
{
  Expression *predicate;
} Filter;

typedef enum _WindowAggregationType {
  MIN,
  MAX,
  SUM,
  AVG,
  COUNT
} WindowAggregationType;

typedef enum _WindowSizeType {
  TIMEBASED,
  COUNTBASED
} WindowSizeType;

typedef struct _Window
{
  int size;
  WindowSizeType sizeType;
  WindowAggregationType aggregationType;
  int startAttribute;
  int endAttribute;
  int resultAttribute;
  int readAttribute;
} Window;

typedef struct _Operation
{
  union {
    Map *map;
    Filter *filter;
    Window *window;
  } operation;
  int unionCase;
} Operation;

typedef struct _Query
{
  Operation *operations;
  int amount;
} Query;

typedef struct _Message
{
  Query *queries;
  int amount;
} Message;

typedef struct _QueryResponse
{
  int id;
  Instruction *response;
  int amount;
} QueryResponse;

typedef struct _OutputMessage
{
  QueryResponse* responses;
  int amount;
} OutputMessage;

Number bin_op(Number n1, Number n2, ExpressionInstruction op);
Number un_op(Number n1, ExpressionInstruction op);

#endif