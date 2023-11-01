#ifndef EXECUTION_H
#define EXECUTION_H

#include "operators.h"

void executeQueries(Message message, OutputMessage *out, Env * env);
void executeQuery(Query query, QueryResponse *out, Env * env);

void free_output_message(OutputMessage *out);

#endif // EXECUTION_H
