#include <stdio.h>
#include "assert.h"
#include "execution.h"
#include "operators.h"
#include "expression.h"
#include <log.h>
static Expression exp;

bool executeQuery(TerraProtocol_Query *query, Stack *stack)
{
  bool filter_triggered = false;
  for (int i = 0; i < query->operations_count && !filter_triggered; i++)
  {

    switch (query->operations[i].which_operation)
    {
    case TerraProtocol_Operation_map_tag:
    {
      // map
      TerraProtocol_MapOperation *map = &query->operations[i].operation.map;
      expression_init_expression(&exp, &(map->function), stack);

      Number number = expression_call(&exp);

      // Set env value
      env_set_value(map->attribute, number);
      break;
    }
    case TerraProtocol_Operation_filter_tag:
    {
      // filter
      TerraProtocol_FilterOperation *filter = &query->operations[i].operation.filter;
      expression_init_expression(&exp, &(filter->predicate), stack);
      Number number = expression_call(&exp);

      if (is_false(&number))
      {
        filter_triggered = true;
      }
      break;
    }

    case TerraProtocol_Operation_window_tag:
      // window
      LOG_ERROR("[execution.c] Window operation not yet supported");
      break;

    default:
      break;
    }

    // after each execution the stack should be empty or 1
    // empty stack after maps. Possible one after filter
    assert(stack->top == -1 || stack->top == 0 || stack->top == 1);
  }
  return !filter_triggered;
}