#include <math.h>
#include <stdio.h>

#include "expression.h"
#include "../stack/stack.h"
#include "../operators/operators.h"
#include "../environment/environment.h"
#include "../number/number.h"

// TODO: How do we handle functions that return doubles?
// TODO: Fix linker errors with math.h functions.

void _CONST(Expression *e)
{
    // push the next value from program as data to the stack
    Number val;
    Instruction currentInstruction = e->program[++e->pc];

    //TODO: check if 1 can be replaced with enum for instance?
    if(currentInstruction.unionCase == 1){
        val.type._uint32 = currentInstruction.data._uint32;
    }
    else if (currentInstruction.unionCase == 2){
        val.type._int = currentInstruction.data._int;
    }
    else if (currentInstruction.unionCase == 3) {
        val.type._float = currentInstruction.data._float;
    }
    else if (currentInstruction.unionCase == 4) {
        val.type._double = currentInstruction.data._double;
    }

    val.unionCase = currentInstruction.unionCase;


    push(e->stack, val);
}

void _VAR(Expression *e)
{
    //TODO: check index is an int.
    int index = e->program[++e->pc].data._int;
    Number val = get_value(e->env, index);
    push(e->stack, val);
}

void _AND(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    Number val = bin_op(l1, l2, '&');

    push(e->stack, val);
}

void _OR(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    
    Number val = bin_op(l1, l2, '|');

    push(e->stack, val);
}

void _NOT(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "not");
    push(e->stack, val);
}

void _LT(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    
    Number val;
    val.unionCase = 2; // int

    int res = compare(l1, l2);
    
    if (res == -1) {
        val.type._int = 1;
    } else {
        val.type._int = 0;
    }

    push(e->stack, val);
}

void _GT(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    
    Number val;
    val.unionCase = 2;

    int res = compare(l1, l2);

    if (res == 1) {
        val.type._int = 1;
    } else {
        val.type._int = 0;
    }

    push(e->stack, val);
}

void _EQ(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    
    Number val;
    val.unionCase = 2;

    int res = compare(l1, l2);

    if (res == 0) {
        val.type._int = 1;
    } else {
        val.type._int = 0;
    }
    
    push(e->stack, val);
}

void _ADD(Expression *e)
{
    Number val;
    
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    val = bin_op(l1, l2, '+');
    
    push(e->stack, val);
}

void _SUB(Expression *e)
{
    Number val;
    
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    val = bin_op(l1, l2, '-');

    push(e->stack, val);
}

void _MUL(Expression *e)
{
    Number val;
    
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    val = bin_op(l1, l2, '*');

    push(e->stack, val);
}

void _DIV(Expression *e)
{
    Number val;
    
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    val = bin_op(l1, l2, '/');

    push(e->stack, val);
}

void _MOD(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    //TODO: allow for uint32 to be modded
    if(l1.unionCase != 2 || l2.unionCase != 2){
        return; //TODO: Throw error (not int)
    }
    
    Number val;

    int res = l1.type._int % l2.type._int;

    val.type._int = res;
    val.unionCase = 2;
    
    push(e->stack, val);
}

void _LOG(Expression *e)
{
    Number l1 = pop(e->stack);
    Number res = un_op(l1, "log");
    push(e->stack, res);
}

void _POW(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    Number res = bin_op(l1, l2, '^');
    push(e->stack, res);
}

void _SQRT(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "sqrt");
    push(e->stack, val);
}

void _EXP(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "exp");
    push(e->stack, val);
}

void _CEIL(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "ceil");
    push(e->stack, val);
}

void _FLOOR(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "floor");
    push(e->stack, val);
}

void _ROUND(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "round");
    push(e->stack, val);
}

void _ABS(Expression *e)
{
    Number l1 = pop(e->stack);
    Number val = un_op(l1, "abs");
    push(e->stack, val);
}

void _LTEQ(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);
    
    Number val;
    val.unionCase = 2;

    int res = compare(l1, l2);

    // if compare returns 1 we know that l1 is greater than l2,
    // otherwise it is less than or equal to l2.
    if (res == 1) {
        val.type._int = 0;
    } else {
        val.type._int = 1;
    }


    push(e->stack, val);
}

void _GTEQ(Expression *e)
{
    Number l2 = pop(e->stack);
    Number l1 = pop(e->stack);

    Number val;
    val.unionCase = 2;

    int res = compare(l1, l2);

    // if compare returns -1 we know that l1 is less than l2,
    // otherwise it is greater than or equal to l2.
    if (res == -1)
    {
        val.type._int = 0;
    }
    else
    {
        val.type._int = 1;
    }

    push(e->stack, val);
}

void execute_next(Expression *e)
{
    switch (e->program[e->pc].data._instruction)
    {
    case 0:
        _CONST(e);
        break;
    case 1:
        _VAR(e);
        break;
    case 2:
        _AND(e);
        break;
    case 3:
        _OR(e);
        break;
    case 4:
        _NOT(e);
        break;
    case 5:
        _LT(e);
        break;
    case 6:
        _GT(e);
        break;
    case 7:
        _EQ(e);
        break;
    case 8:
        _ADD(e);
        break;
    case 9:
        _SUB(e);
        break;
    case 10:
        _MUL(e);
        break;
    case 11:
        _DIV(e);
        break;
    case 12:
        _MOD(e);
        break;
    case 13:
        _LOG(e);
        break;
    case 14:
        _POW(e);
        break;
    case 15:
        _SQRT(e);
        break;
    case 16:
        _EXP(e);
        break;
    case 17:
        _CEIL(e);
        break;
    case 18:
        _FLOOR(e);
        break;
    case 19:
        _ROUND(e);
        break;
    case 20:
        _ABS(e);
        break;
    case 21:
        _LTEQ(e);
        break;
    case 22:
        _GTEQ(e);
        break;
    }
}

Number call(Expression *e)
{
    e->pc = 0;
    while (e->pc < e->p_size)
    {
        execute_next(e);
        e->pc++;
    }
    Number val = pop(e->stack);
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