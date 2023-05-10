#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "number.h"
#include "operators.h"

Number call(Expression *e);
void _CONST(Expression *e);
void _VAR(Expression *e);
void _AND(Expression *e);
void _OR(Expression *e);
void _NOT(Expression *e);
void _LT(Expression *e);
void _GT(Expression *e);
void _EQ(Expression *e);
void _ADD(Expression *e);
void _SUB(Expression *e);
void _MUL(Expression *e);
void _DIV(Expression *e);
void _MOD(Expression *e);
void _LOG(Expression *e);
void _POW(Expression *e);
void _SQRT(Expression *e);
void _EXP(Expression *e);
void _CEIL(Expression *e);
void _FLOOR(Expression *e);
void _ROUND(Expression *e);
void _ABS(Expression *e);
void _LTEQ(Expression *e);
void _GTEQ(Expression *e);
void execute_next(Expression *e);
#endif