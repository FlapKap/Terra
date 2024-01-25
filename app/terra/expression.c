#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "expression.h"
#include "stack.h"
#include "operators.h"
#include "environment.h"
#include "number.h"
#include "print_utils.h"
#include "log.h"

void expression_init_expression(Expression* e, TerraProtocol_Expression* program, Stack* stack) {
    e->program = program;
    e->stack = stack;
}

// TODO: add debug statements to the code

static void _CONST(Expression *e)
{
    // push the next value from program as data to the stack
    Number val = { 0 };
    TerraProtocol_Data* nextInstruction = &(e->program->instructions[++(e->pc)]);

    copy_instruction_to_number(nextInstruction, &val);
    
    stack_push(e->stack, val);
}

static void _VAR(Expression *e)
{
    //TODO: assert data is an uint
    uint32_t index = e->program->instructions[++e->pc].data._uint32;
    Number val;
    bool valid = env_get_value(index, &val);
    if(!valid){
        LOG_ERROR("[expression.c] _VAR: Invalid variable index: %" PRIu32 "", index);
    }
    stack_push(e->stack, val);
}

static void _AND(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_AND, &val);

    stack_push(e->stack, val);
}

static void _OR(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_OR, &val);

    stack_push(e->stack, val);
}

static void _NOT(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_NOT, &val);
    stack_push(e->stack, val);
}

static void _LT(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_LT, &val);

    stack_push(e->stack, val);
}

static void _GT(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_GT, &val);
    stack_push(e->stack, val);
}

static void _EQ(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_EQ, &val);
    
    stack_push(e->stack, val);
}

static void _ADD(Expression *e)
{   
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_ADD, &val);
    
    stack_push(e->stack, val);
}

static void _SUB(Expression *e)
{
    
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_SUB, &val);

    stack_push(e->stack, val);
}

static void _MUL(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_MUL, &val);

    stack_push(e->stack, val);
}

static void _DIV(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_DIV, &val);

    stack_push(e->stack, val);
}

static void _MOD(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_MOD, &val);

    stack_push(e->stack, val);
}

static void _LOG(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_LOG, &val);
    stack_push(e->stack, val);
}

static void _POW(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_POW, &val);
    stack_push(e->stack, val);
}

static void _SQRT(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_SQRT, &val);
    stack_push(e->stack, val);
}

static void _EXP(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_EXP, &val);
    stack_push(e->stack, val);
}

static void _CEIL(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_CEIL, &val);
    stack_push(e->stack, val);
}

static void _FLOOR(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_FLOOR, &val);
    stack_push(e->stack, val);
}

static void _ROUND(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_ROUND, &val);
    stack_push(e->stack, val);
}

static void _ABS(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_ABS, &val);
    stack_push(e->stack, val);
}

static void _LTEQ(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_LTEQ, &val);

    stack_push(e->stack, val);
}

static void _GTEQ(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_GTEQ, &val);

    stack_push(e->stack, val);
}

static void execute_next(Expression *e)
{
    if (LOG_LEVEL >= LOG_DEBUG) {
        print_terraprotocol_data(&e->program->instructions[e->pc]);
        puts("\n");
    }
    switch (e->program->instructions[e->pc].data.instruction)
    {
    case TerraProtocol_CONST:
        _CONST(e);
        break;
    case TerraProtocol_VAR:
        _VAR(e);
        break;
    case TerraProtocol_AND:
        _AND(e);
        break;
    case TerraProtocol_OR:
        _OR(e);
        break;
    case TerraProtocol_NOT:
        _NOT(e);
        break;
    case TerraProtocol_LT:
        _LT(e);
        break;
    case TerraProtocol_GT:
        _GT(e);
        break;
    case TerraProtocol_EQ:
        _EQ(e);
        break;
    case TerraProtocol_ADD:
        _ADD(e);
        break;
    case TerraProtocol_SUB:
        _SUB(e);
        break;
    case TerraProtocol_MUL: 
        _MUL(e);
        break;
    case TerraProtocol_DIV:
        _DIV(e);
        break;
    case TerraProtocol_MOD:
        _MOD(e);
        break;
    case TerraProtocol_LOG:
        _LOG(e);
        break;
    case TerraProtocol_POW:
        _POW(e);
        break;
    case TerraProtocol_SQRT:
        _SQRT(e);
        break;
    case TerraProtocol_EXP:
        _EXP(e);
        break;
    case TerraProtocol_CEIL:
        _CEIL(e);
        break;
    case TerraProtocol_FLOOR:
        _FLOOR(e);
        break;
    case TerraProtocol_ROUND:
        _ROUND(e);
        break;
    case TerraProtocol_ABS:
        _ABS(e);
        break;
    case TerraProtocol_LTEQ:
        _LTEQ(e);
        break;
    case TerraProtocol_GTEQ:
        _GTEQ(e);
        break;
    }
}

Number expression_call(Expression *e)
{
    if (LOG_LEVEL >= LOG_DEBUG) {
        print_expression(e);
    }
    e->pc = 0;
    while (e->pc < e->program->instructions_count)
    {
        execute_next(e);
        ++e->pc;
    }

    Number val = stack_pop(e->stack);
    return val;
}

// Instructions:
//  Data
//      CONST = 0
//      VAR = 1
//  Logical
//      AND = 2
//      OR = 3
//      NOT = 4
//  Relational
//      LT = 5
//      GT = 6
//      EQ = 7
//  Relational expanded
//      LTEQ = 21
//      GTEQ = 22
//  Arithmetic
//      ADD = 8
//      SUB = 9
//      MUL = 10
//      DIV = 11
//      MOD = 12
//      LOG = 13
//      POW = 14
//      SQRT = 15
//      EXP = 16
//      CEIL = 17
//      FLOOR = 18
//      ROUND = 19
//      ABS = 20
