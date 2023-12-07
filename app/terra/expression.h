#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "number.h"
#include "operators.h"

void expression_init_expression(Expression* e, TerraProtocol_Expression* program, Env* env, Stack* stack);

Number expression_call(Expression *e);

#endif
