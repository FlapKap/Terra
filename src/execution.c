#include <stdio.h>
#include "execution.h"
#include "operators.h"
#include "expression.h"
#include "environment.h"



void executeQueries(TerraProtocol_Message* message, TerraProtocol_Output *out, Env * env){
  for (int i = 0; i < message->queries_count; i++) {
    executeQuery(&message->queries[i], &out->responses[i], env);
  }

  //TODO: right now we generate 1 response per query. what if no responses are needed?
  out->responses_count = message->queries_count;
}

void executeQuery(TerraProtocol_Query* query, TerraProtocol_Output_QueryResponse *out, Env * env){
  for(int i = 0; i < query->operations_count; i++){
    clear_stack(env);
    switch (query->operations[i].which_operation)
    {
    case TerraProtocol_Operation_map_tag:
      query->operations[i].operation.map.function.instructions = env->stack;
      Number number = call(query->operations[i].operation.map->expression);
 
      // Set env value
      set_value(env, query->operations[i].operation.map->attribute, number);

      Instruction *instruction = (Instruction *) calloc(sizeof(Instruction), 1);

      copy_number_to_instruction(&number, instruction);
      out->response = instruction;
      out->amount = 1;
      break;
    
    default:
      break;
    }
    if(query->operations[i].unionCase == 0){
      // map

    } else if(query->operations[i].unionCase == 1){
      // filter
      query->operations[i].operation.filter->predicate->stack = env->stack;

      Number number = call(query->operations[i].operation.filter->predicate);

      Instruction *instruction = (Instruction *) calloc(sizeof(Instruction), 1);
      //TODO: this seems like a bug. Shouldnt filter stop execution?
      copy_number_to_instruction(&number, instruction);
      out->response = instruction;
      out->amount = 1;
    } else if(query->operations[i].unionCase == 2){
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
