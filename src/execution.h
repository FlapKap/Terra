#ifndef EXECUTION_H
#define EXECUTION_H

#include "operators.h"

void executeQueries(TerraProtocol_Message* message, TerraProtocol_Output *out, Env * env);
void executeQuery(TerraProtocol_Query* query, TerraProtocol_Output_QueryResponse *out, Env * env);

//void free_output_message(OutputMessage *out);

#endif // EXECUTION_H
