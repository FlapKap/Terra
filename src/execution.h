#ifndef EXECUTION_H
#define EXECUTION_H

#include "operators.h"
#include "stack.h"
#include "environment.h"
#include <terraprotocol.pb.h>

void executeQueries(TerraProtocol_Message* message, Env * env, Stack * stack);
void executeQuery(TerraProtocol_Query* query, Env * env, Stack * stack);


#endif // EXECUTION_H
