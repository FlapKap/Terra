#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <terraprotocol.pb.h>
#include "operators.h"
#include "number.h"
#include "configuration.h"
#ifdef MODULE_PM_LAYERED
#include "pm_layered.h"
#endif

void print_configuration(TerraConfiguration* config);
void print_terraprotocol_expression_instruction(TerraProtocol_ExpressionInstructions instr);
void print_terraprotocol_data(const TerraProtocol_Data* instr);
void print_terraprotocol_data_array(const TerraProtocol_Data* instruction, size_t size);
void print_number_value(Number val);
void print_number_value_and_ucase(Number number);
void print_stack(const Stack* stack);
void print_expression(const Expression* expression);
void print_terraprotocol_map(const TerraProtocol_MapOperation* map);
void print_terraprotocol_filter(const TerraProtocol_FilterOperation* filter);
void print_terraprotocol_aggregation(const TerraProtocol_Aggregation* agg);
void print_terraprotocol_tumblingwindowoperation(const TerraProtocol_TumblingWindowOperation* tumbling);
void print_terraprotocol_slidingwindowoperation(const TerraProtocol_SlidingWindowOperation* sliding);
void print_terraprotocol_thresholdwindowoperation(const TerraProtocol_ThresholdWindowOperation* threshold);
void print_terraprotocol_windowoperation(const TerraProtocol_WindowOperation* window);
void print_terraprotocol_window(const TerraProtocol_WindowOperation* window);
void print_terraprotocol_operation(const TerraProtocol_Operation* operation);
void print_terraprotocol_query(const TerraProtocol_Query* query);
void print_terraprotocol_message(const TerraProtocol_Message* message);
//void print_terraprotocol_query_response(const TerraProtocol_Output_QueryResponse* response);
void print_terraprotocol_output_message(const TerraProtocol_Output* message);
#ifdef MODULE_PM_LAYERED
void print_blockers(pm_blocker_t *blockers, size_t size);
#endif
void print_device_info(void);
void print_build_info(void);

#endif // PRINT_UTILS_H
