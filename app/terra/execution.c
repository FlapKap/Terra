#include <stdio.h>
#include "assert.h"
#include "execution.h"
#include "operators.h"
#include "environment.h"
#include "expression.h"
#include "configuration.h"
#include <log.h>
#include <terraprotocol.pb.h>
#include <ztimer.h>

static Expression exp;

extern TerraConfiguration config;

static bool executeAggregation(TerraProtocol_Aggregation *aggregation, WindowData *window_data)
{
  Number input;

  env_get_value(aggregation->onAttribute, &input);

  switch (aggregation->aggregationType)
  {
  case TerraProtocol_MIN:
  {
    if (number_compare(&input, &window_data->aggregation_value) < 0)
    {
      window_data->aggregation_value = input;
    }
    break;
  }
  case TerraProtocol_MAX:
  {

    if (number_compare(&input, &window_data->aggregation_value) > 0)
    {
      window_data->aggregation_value = input;
    }
    break;
  }
  case TerraProtocol_SUM:
  {
    number_add(&window_data->aggregation_value, &input, &window_data->aggregation_value);
    break;
  }
  case TerraProtocol_AVG:
  {
    // done by new_avg = old_avg +  ((new_val - old_avg) / (count+1))
    // https://math.stackexchange.com/questions/22348/how-to-add-and-subtract-values-from-an-average
    Number temp;
    Number count = {.type._uint32 = window_data->count, .unionCase = NUMBER_UINT32};

    number_sub(&input, &window_data->aggregation_value, &temp);
    number_div(&temp, &count, &temp);
    number_add(&window_data->aggregation_value, &temp, &window_data->aggregation_value);
    break;
  }
  case TerraProtocol_COUNT:
    // need to do nothing as the count is kept track of already
    break;
  }
  return true;
}

static bool executeTumblingWindow(TerraProtocol_TumblingWindowOperation *tumbling, TerraProtocol_Aggregation *aggregation, WindowData *window_data)
{

  // check if its the first execution
  if (window_data->state == WINDOW_STATE_NOT_INITIALIZED)
  {
    // first time executing the window so we need to initialize a bunch of values
    window_data->state = WINDOW_STATE_RUNNING;
    window_data->count = 0;

    env_get_value(aggregation->onAttribute, &window_data->start_value);
  }

  // check if we need to execute the aggregation
  // fallback without RTC. approximate by multiplying number of executions by epoch time
  // TODO: right now we have RTC so we could do this properly
  uint32_t current_time = window_data->count * (EXECUTION_EPOCH_S * 1000);
  if (current_time > (uint32_t)tumbling->size_ms)
  {
    executeAggregation(aggregation, window_data);
    window_data->count++;
    env_get_value(aggregation->onAttribute, &window_data->end_value);
  }
  else
  {
    // we are done
    window_data->state = WINDOW_STATE_FINISHED;
  }
  return true;
}

static bool executeWindow(TerraProtocol_WindowOperation *window, WindowData *window_data)
{
  if (!window->has_aggregation)
  {
    LOG_ERROR("[execution.c] No aggregation in window");
    return false; // early exit if no aggregation is there
  }

  switch (window->which_WindowOperation)
  {
  case TerraProtocol_WindowOperation_tumbling_tag:
  {
    TerraProtocol_TumblingWindowOperation *tumbling = &window->WindowOperation.tumbling;
    TerraProtocol_Aggregation *aggregation = &window->aggregation;
    return executeTumblingWindow(tumbling, aggregation, window_data);
    break;
  }
  case TerraProtocol_WindowOperation_sliding_tag:
  {
    LOG_ERROR("[execution.c] Sliding window not implemented");
    return false;
    break;
  }
  case TerraProtocol_WindowOperation_threshold_tag:
  {
    LOG_ERROR("[execution.c] Threshold window not implemented");
    return false;
    break;
  }
  default:
  {
    LOG_ERROR("[execution.c] Unknown window operation");
    return false;
    break;
  }
  }
}
static uint8_t current_window_index = 0;
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

      if (number_is_false(&number))
      {
        filter_triggered = true;
      }
      break;
    }

    case TerraProtocol_Operation_window_tag:
    {
      // window
      TerraProtocol_WindowOperation *window = &query->operations[i].operation.window;
      executeWindow(window, &config.window_data[current_window_index]);
      current_window_index++;
      break;
    }
    default:
    {
      LOG_ERROR("[execution.c] Unknown operation");
      break;
    }
    }

    // after each execution the stack should be empty or 1
    // empty stack after maps. Possible one after filter
    assert(stack->top == -1 || stack->top == 0 || stack->top == 1);
  }
  return !filter_triggered;
}