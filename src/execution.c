#include <stdio.h>
#include "execution.h"
#include "operators.h"
#include "expression.h"
#include "environment.h"

void executeQueries(Message message, OutputMessage *out, Env * env){

  QueryResponse *responses = (QueryResponse *) malloc(sizeof(QueryResponse) * message.amount);

  for (int i = 0; i < message.amount; i++) {
    QueryResponse current;
    executeQuery(message.queries[i], &current, env);
    responses[i] = current;
  }

  out->responses = responses;
  out->amount = message.amount;
}

void executeQuery(Query query, QueryResponse *out, Env * env){
  for(int i = 0; i < query.amount; i++){
    clear_stack(env);
    if(query.operations[i].unionCase == 0){
      // map
      query.operations[i].operation.map->expression->stack = env->stack;
      Number num = call(query.operations[i].operation.map->expression);

      // Set env value
      set_value(env, query.operations[i].operation.map->attribute, num);

      Instruction *instr = (Instruction *) malloc(sizeof(Instruction));

      instr->unionCase = num.unionCase;
      if (num.unionCase == 1){
        instr->data._uint32 = num.type._uint32;
      } else if (num.unionCase == 2){
        instr->data._int = num.type._int;
      } else if (num.unionCase == 3){
        instr->data._float = num.type._float;
      } else if (num.unionCase == 4){
        instr->data._double = num.type._double;
      } else {
        printf("Unknown unioncase execute map!\n");
      }
      
      out->response = instr;
      out->amount = 1;
    } else if(query.operations[i].unionCase == 1){
      // filter
      query.operations[i].operation.filter->predicate->stack = env->stack;

      Number num = call(query.operations[i].operation.filter->predicate);

      Instruction *instr = (Instruction *) malloc(sizeof(Instruction));
      instr->unionCase = num.unionCase;
      if (num.unionCase == 1){
        instr->data._uint32 = num.type._uint32;
      } else if (num.unionCase == 2){
        instr->data._int = num.type._int;
      } else if (num.unionCase == 3){
        instr->data._float = num.type._float;
      } else if (num.unionCase == 4){
        instr->data._double = num.type._double;
      } else {
        printf("Unknown unioncase execute filter!\n");
      }
      out->response = instr;
      out->amount = 1;
    } else if(query.operations[i].unionCase == 2){
      // window
      puts("Window operation not yet supported");
      return;
    }
  }
}

void free_output_message(OutputMessage *out){
  for(int i = 0; i < out->amount; i++){
    free(out->responses[i].response);
  }
  free(out->responses);
}