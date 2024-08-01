#ifndef EXECUTION_H
#define EXECUTION_H

#include "operators.h"
#include "stack.h"
#include "environment.h"
#include <terraprotocol.pb.h>
#include <stdint.h>


/**
 * @brief Executes the given query on the given stack. Assumes the stack is empty and
 * if sensor values are used that they are already in the environment.
 * @param query The Query to execute
 * @param stack The stack to execute on
 * @return true if the query was executed completely. False if it was cancelled at some point. E.g. due to a filter or unfinished window.
 */
bool executeQuery(TerraProtocol_Query* query, Stack * stack);



#endif // EXECUTION_H
