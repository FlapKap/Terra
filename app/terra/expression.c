#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include "operators.h"
#include "expression.h"

#include <terraprotocol.pb.h>

#include "stack.h"

#include "environment.h"
#include "number.h"
#include "print_utils.h"
#include "log.h"

void expression_init_expression(Expression* e, TerraProtocol_Expression* program, Stack* stack) {
    e->program = program;
    e->stack = stack;
}

// TODO: add debug statements to the code

static void expression_CONST(Expression* e)
{
    // push the next value from program as data to the stack
    Number val = { 0 };
    TerraProtocol_Data* nextInstruction = &(e->program->instructions[++(e->pc)]);
    assert(nextInstruction->which_data != TerraProtocol_Data_instruction_tag); // data is an instruction and not a valid value
    copy_instruction_to_number(nextInstruction, &val);
    
    stack_push(e->stack, val);
}

static void expression_VAR(Expression* e)
{
    TerraProtocol_Data* nextInstruction = &(e->program->instructions[++(e->pc)]);
    //assert next instruction is uint
    assert(nextInstruction->which_data == TerraProtocol_Data__uint8_tag ||
           nextInstruction->which_data == TerraProtocol_Data__uint16_tag ||
           nextInstruction->which_data == TerraProtocol_Data__uint32_tag);

    uint32_t index = nextInstruction->data._uint32;
    Number val = { 0 };
    bool valid = env_get_value(index, &val);
    if(!valid){
        LOG_ERROR("[expression.c] _VAR: Invalid variable index: %" PRIu32 "", index);
    }
    stack_push(e->stack, val);
}

static void expression_AND(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_AND, &val);

    stack_push(e->stack, val);
}

static void expression_OR(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_OR, &val);

    stack_push(e->stack, val);
}

static void expression_NOT(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_NOT, &val);
    stack_push(e->stack, val);
}

static void expression_LT(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_LT, &val);

    stack_push(e->stack, val);
}

static void expression_GT(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_GT, &val);
    stack_push(e->stack, val);
}

static void expression_EQ(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_EQ, &val);
    
    stack_push(e->stack, val);
}

static void expression_ADD(Expression *e)
{   
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_ADD, &val);
    
    stack_push(e->stack, val);
}

static void expression_SUB(Expression *e)
{
    
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_SUB, &val);

    stack_push(e->stack, val);
}

static void expression_MUL(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_MUL, &val);

    stack_push(e->stack, val);
}

static void expression_DIV(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_DIV, &val);

    stack_push(e->stack, val);
}

static void expression_MOD(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);

    Number val;
    bin_op(&left, &right, TerraProtocol_MOD, &val);

    stack_push(e->stack, val);
}

static void expression_LOG(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_LOG, &val);
    stack_push(e->stack, val);
}

static void expression_POW(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    Number val;
    bin_op(&left, &right, TerraProtocol_POW, &val);
    stack_push(e->stack, val);
}

static void expression_SQRT(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_SQRT, &val);
    stack_push(e->stack, val);
}

static void expression_EXP(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_EXP, &val);
    stack_push(e->stack, val);
}

static void expression_CEIL(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_CEIL, &val);
    stack_push(e->stack, val);
}

static void expression_FLOOR(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_FLOOR, &val);
    stack_push(e->stack, val);
}

static void expression_ROUND(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_ROUND, &val);
    stack_push(e->stack, val);
}

static void expression_ABS(Expression *e)
{
    Number left = stack_pop(e->stack);
    Number val;
    un_op(&left, TerraProtocol_ABS, &val);
    stack_push(e->stack, val);
}

static void expression_LTEQ(Expression *e)
{
    Number right = stack_pop(e->stack);
    Number left = stack_pop(e->stack);
    
    Number val;
    bin_op(&left, &right, TerraProtocol_LTEQ, &val);

    stack_push(e->stack, val);
}

static void expression_GTEQ(Expression *e)
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
    }
    TerraProtocol_Data* instruction = &e->program->instructions[e->pc];
    assert(instruction->which_data == TerraProtocol_Data_instruction_tag); //ensure this is an instruction, not a value
    switch (instruction->data.instruction)
    {
    case TerraProtocol_CONST:
        expression_CONST(e);
        break;
    case TerraProtocol_VAR:
        expression_VAR(e);
        break;
    case TerraProtocol_AND:
        expression_AND(e);
        break;
    case TerraProtocol_OR:
        expression_OR(e);
        break;
    case TerraProtocol_NOT:
        expression_NOT(e);
        break;
    case TerraProtocol_LT:
        expression_LT(e);
        break;
    case TerraProtocol_GT:
        expression_GT(e);
        break;
    case TerraProtocol_EQ:
        expression_EQ(e);
        break;
    case TerraProtocol_ADD:
        expression_ADD(e);
        break;
    case TerraProtocol_SUB:
        expression_SUB(e);
        break;
    case TerraProtocol_MUL: 
        expression_MUL(e);
        break;
    case TerraProtocol_DIV:
        expression_DIV(e);
        break;
    case TerraProtocol_MOD:
        expression_MOD(e);
        break;
    case TerraProtocol_LOG:
        expression_LOG(e);
        break;
    case TerraProtocol_POW:
        expression_POW(e);
        break;
    case TerraProtocol_SQRT:
        expression_SQRT(e);
        break;
    case TerraProtocol_EXP:
        expression_EXP(e);
        break;
    case TerraProtocol_CEIL:
        expression_CEIL(e);
        break;
    case TerraProtocol_FLOOR:
        expression_FLOOR(e);
        break;
    case TerraProtocol_ROUND:
        expression_ROUND(e);
        break;
    case TerraProtocol_ABS:
        expression_ABS(e);
        break;
    case TerraProtocol_LTEQ:
        expression_LTEQ(e);
        break;
    case TerraProtocol_GTEQ:
        expression_GTEQ(e);
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
