#include <stdio.h>
#include "assert.h"
#include "execution.h"
#include "operators.h"
#include "expression.h"
static Expression exp;

void executeQuery(TerraProtocol_Query *query, Env *env, Stack *stack)
{
  stack_clear_stack(stack);
  //env_clear_env(env);

  bool filter_triggered = false;
  for (int i = 0; i < query->operations_count && !filter_triggered; i++)
  {

    switch (query->operations[i].which_operation)
    {
    case TerraProtocol_Operation_map_tag:
    {
      // map
      TerraProtocol_MapOperation *map = &query->operations[i].operation.map;
      expression_init_expression(&exp, &(map->function), env, stack);

      Number number = expression_call(&exp);

      // Set env value
      env_set_value(env, map->attribute, number);
      break;
    }
    case TerraProtocol_Operation_filter_tag:
    {
      // filter
      TerraProtocol_FilterOperation *filter = &query->operations[i].operation.filter;
      expression_init_expression(&exp, &(filter->predicate), env, stack);
      Number number = expression_call(&exp);

      if (is_false(&number))
      {
        filter_triggered = true;
      }
      break;
    }

    case TerraProtocol_Operation_window_tag:
      // window
      puts("Window operation not yet supported");
      break;

    default:
      break;
    }

    // after each execution the stack should be empty or 1
    // empty stack after maps. Possible one after filter
    assert(stack->size == 0 || stack->size == 1);
  }
}

void executeQueries(TerraProtocol_Message *message, Env *env, Stack *stack)
{
  for (int i = 0; i < message->queries_count; i++)
  {
    executeQuery(&message->queries[i], env, stack);
  }
}
