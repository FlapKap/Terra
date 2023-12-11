#include "print_utils.h"
#include <stdio.h>
#include <inttypes.h>
#include "lorawan.h"


void print_configuration(TerraConfiguration* config){
    printf(
            "---Configuration start ---\n"
            "Loop counter: %" PRIu32 "\n",
            config->loop_counter
            );

    if (config->query != NULL) {
        print_terraprotocol_message(config->query);
    }
#if !(defined(APPLICATION_RUN_TEST) || defined(DISABLE_LORA))
    if (config->loramac != NULL) {
        lorawan_print_connection_info();
    }
#endif
    printf("---Configuration end ---\n");
}

// #lizard forgives
void print_terraprotocol_expression_instruction(TerraProtocol_ExpressionInstructions instr)
{
    switch (instr)
    {
    case TerraProtocol_CONST:
        printf("CONST");
        break;
    case TerraProtocol_VAR:
        printf("VAR");
        break;
    case TerraProtocol_AND:
        printf("AND");
        break;
    case TerraProtocol_OR:
        printf("OR");
        break;
    case TerraProtocol_NOT:
        printf("NOT");
        break;
    case TerraProtocol_LT:
        printf("LT");
        break;
    case TerraProtocol_GT:
        printf("GT");
        break;
    case TerraProtocol_EQ:
        printf("EQ");
        break;
    case TerraProtocol_ADD:
        printf("ADD");
        break;
    case TerraProtocol_SUB:
        printf("SUB");
        break;
    case TerraProtocol_MUL:
        printf("MUL");
        break;
    case TerraProtocol_DIV:
        printf("DIV");
        break;
    case TerraProtocol_MOD:
        printf("MOD");
        break;
    case TerraProtocol_LOG:
        printf("LOG");
        break;
    case TerraProtocol_POW:
        printf("POW");
        break;
    case TerraProtocol_SQRT:
        printf("SQRT");
        break;
    case TerraProtocol_EXP:
        printf("EXP");
        break;
    case TerraProtocol_CEIL:
        printf("CEIL");
        break;
    case TerraProtocol_FLOOR:
        printf("FLOOR");
        break;
    case TerraProtocol_ROUND:
        printf("ROUND");
        break;
    case TerraProtocol_ABS:
        printf("ABS");
        break;
    case TerraProtocol_LTEQ:
        printf("LTEQ");
        break;
    case TerraProtocol_GTEQ:
        printf("GTEQ");
        break;
    default:
        printf("Invalid ExpressionInstruction");
        break;
    }
}

void print_terraprotocol_data(const TerraProtocol_Data *instruction)
{
    switch (instruction->which_data)
    {
    case TerraProtocol_Data_instruction_tag:
        print_terraprotocol_expression_instruction(instruction->data.instruction);
        printf(" (ExpressionInstruction)");
        break;

    case TerraProtocol_Data__uint8_tag:
        printf("%" PRIu32 " (uint8_t)", instruction->data._uint8);
        break;

    case TerraProtocol_Data__uint16_tag:
        printf("%" PRIu32 " (uint16_t)", instruction->data._uint16);
        break;

    case TerraProtocol_Data__uint32_tag:
        printf("%" PRIu32 " (uint32_t)", instruction->data._uint32);
        break;

    case TerraProtocol_Data__int8_tag:
        printf("%" PRIi32 " (int8_t)", instruction->data._int8);
        break;

    case TerraProtocol_Data__int16_tag:
        printf("%" PRIi32 " (int16_t)", instruction->data._int16);
        break;

    case TerraProtocol_Data__int32_tag:
        printf("%" PRIi32 " (int32_t)", instruction->data._int32);
        break;

    case TerraProtocol_Data__float_tag:
        printf("%f (float)", instruction->data._float);
        break;

    case TerraProtocol_Data__double_tag:
        printf("%f (double)", instruction->data._double);
        break;
    }
}

void print_terraprotocol_data_array(const TerraProtocol_Data *instruction, size_t size)
{
    printf("[");
    for (size_t i = 0; i < size; i++)
    {
        print_terraprotocol_data(&instruction[i]);
        printf(", ");
    }
    printf("]");
}

void print_number_value(Number val)
{
    switch (val.unionCase)
    {
    case NUMBER_UINT32:

        printf("%" PRIu32, val.type._uint32);
        break;
    case NUMBER_INT32:
        printf("%d", val.type._int);
        break;
    case NUMBER_FLOAT:
        printf("%f", val.type._float);
        break;
    case NUMBER_DOUBLE:
        printf("%lf", val.type._double);
        break;
    default:
        printf("Error: Invalid unionCase: %" PRIu8, val.unionCase);
        break;
    }
}

void print_number_value_and_ucase(Number number)
{
    switch (number.unionCase)
    {
    case NUMBER_UINT32:
        printf("%" PRIu32 " (uint)", number.type._uint32);
        break;
    case NUMBER_INT32:
        printf("%d (int)", number.type._int);
        break;
    case NUMBER_FLOAT:
        printf("%f (float)", number.type._float);
        break;
    case NUMBER_DOUBLE:
        printf("%lf (double)", number.type._double);
        break;
    default:
        printf("Error: Invalid unionCase: %" PRIu8 "", number.unionCase);
        break;
    }
}

void print_stack(const Stack *stack)
{
    printf(" Stack (top: %d, size: %d):\n", stack->top, stack->size);
    if (stack->top == -1)
    {
        printf("  Empty\n");
    }
    else
    {
        printf("  Elements:\n");
        for (int i = stack->top; i >= 0; i--)
        {
            printf("    %d. ", stack->top - i + 1);
            print_number_value_and_ucase(stack->stack_memory[i]);
            printf("\n");
        }
    }
}

void print_env(const Env *env)
{
    printf("Env (size: %d):\n", env->size);
    printf(" Array:\n");
    for (int i = 0; i < env->size; i++)
    {
        printf("   %d. ", i + 1);
        print_number_value_and_ucase(env->memory[i]);
        printf("\n");
    }
}

void print_expression(const Expression *expression)
{
    printf("     Expression (length: %d, pc: %d):\n", expression->program->instructions_count, expression->pc);
    printf("      Program:\n");
    print_terraprotocol_data_array(expression->program->instructions, expression->program->instructions_count);
    printf("\n");
    printf("      Environment:\n");
    if (expression->env != NULL)
    {
        print_env(expression->env);
    }
    else
    {
        printf("       NULL\n");
    }

    printf("      Stack:\n");
    if (expression->stack != NULL)
    {
        print_stack(expression->stack);
    }
    else
    {
        printf("       NULL\n");
    }
    printf("\n");
}

void print_terraprotocol_expression(const TerraProtocol_Expression *expression)
{
    printf("    Expression (len = %d):\n", expression->instructions_count);
    printf("     Program:\n");
    print_terraprotocol_data_array(expression->instructions, expression->instructions_count);
    printf("\n");
}

void print_terraprotocol_map(const TerraProtocol_MapOperation *map)
{
    printf("    Map (attribute: %" PRIi32 "):\n", map->attribute);
    printf("     Expression:\n");
    print_terraprotocol_expression(&map->function);
}

void print_terraprotocol_filter(const TerraProtocol_FilterOperation *filter)
{
    printf("    Filter:\n");
    printf("     Predicate:\n");
    print_terraprotocol_expression(&filter->predicate);
}

void print_terraprotocol_window_aggregation_type(TerraProtocol_WindowAggregationType type)
{
    switch (type)
    {
    case TerraProtocol_MIN:
        printf("MIN");
        break;
    case TerraProtocol_MAX:
        printf("MAX");
        break;
    case TerraProtocol_SUM:
        printf("SUM");
        break;
    case TerraProtocol_AVG:
        printf("AVG");
        break;
    case TerraProtocol_COUNT:
        printf("COUNT");
        break;
    default:
        printf("Unknown WindowAggregationType");
        break;
    }
}

void print_terraprotocol_window_size_type(TerraProtocol_WindowSizeType type)
{
    switch (type)
    {
    case TerraProtocol_TIMEBASED:
        printf("TIMEBASED");
        break;
    case TerraProtocol_COUNTBASED:
        printf("COUNTBASED");
        break;
    default:
        printf("Unknown WindowSizeType");
        break;
    }
}

void print_terraprotocol_window(const TerraProtocol_WindowOperation *window)
{
    printf("    Window:\n");
    printf("     size: %" PRIi32 "\n", window->size);

    printf("     sizeType: ");
    print_terraprotocol_window_size_type(window->sizeType);
    printf("\n");
    printf("     aggregationType: ");
    print_terraprotocol_window_aggregation_type(window->aggregationType);
    printf("\n");

    printf("     startAttribute: %" PRIi32 "\n", window->startAttribute);
    printf("     endAttribute: %" PRIi32 "\n", window->endAttribute);
    printf("     resultAttribute: %" PRIi32 "\n", window->resultAttribute);
    printf("     readAttribute: %" PRIi32 "\n", window->readAttribute);
}

void print_terraprotocol_operation(const TerraProtocol_Operation *operation)
{
    switch (operation->which_operation)
    {
    case TerraProtocol_Operation_map_tag:
        printf("   Map operation:\n");
        print_terraprotocol_map(&operation->operation.map);
        break;
    case TerraProtocol_Operation_filter_tag:
        printf("   Filter operation:\n");
        print_terraprotocol_filter(&operation->operation.filter);
        break;
    case TerraProtocol_Operation_window_tag:
        printf("   Window operation:\n");
        print_terraprotocol_window(&operation->operation.window);
        break;
    default:
        printf("   Unknown operation\n");
        break;
    }
}

void print_terraprotocol_query(const TerraProtocol_Query *query)
{
    printf(" Query (amount: %d):\n", query->operations_count);
    for (int i = 0; i < query->operations_count; i++)
    {
        printf("  Operation %d:\n", i + 1);
        print_terraprotocol_operation(&query->operations[i]);
    }
}

void print_terraprotocol_message(const TerraProtocol_Message *message)
{
    printf("Message (amount of queries: %d):\n", message->queries_count);
    for (int i = 0; i < message->queries_count; i++)
    {
        printf(" Query %d:\n", i + 1);
        print_terraprotocol_query(&message->queries[i]);
    }
}

void print_terraprotocol_query_response(const TerraProtocol_Output_QueryResponse *response)
{
    printf("Query Response (id: %" PRIi32 ", amount of instructions: %d):\n", response->id, response->response_count);
    for (int i = 0; i < response->response_count; i++)
    {
        printf("Instruction %d:\n", i + 1);
        print_terraprotocol_data(&response->response[i]);
    }
}

void print_terraprotocol_output_message(const TerraProtocol_Output *message)
{
    printf("Output Message (amount of responses: %d):\n", message->responses_count);
    for (int i = 0; i < message->responses_count; i++)
    {
        printf("Response %d:\n", i + 1);
        print_terraprotocol_query_response(&message->responses[i]);
        printf("\n");
    }
}
#ifdef MODULE_PM_LAYERED
void print_blockers(pm_blocker_t *blockers, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        uint8_t blocker_num = blockers->blockers[i];
        printf("Power mode: %u, blockers: %d\n", i, (int)blocker_num);
    }
}
#endif

void print_device_info(void)
{
    printf("Device Information:\n");
    printf("CPU: %s\n", RIOT_CPU);
    printf("Board: %s\n", RIOT_BOARD);
    printf("Riot Version: %s\n", RIOT_VERSION);
#ifdef MODULE_PM_LAYERED
    printf("Number of power modes: %u\n", PM_NUM_MODES);

    printf("current blockers:\n");
    pm_blocker_t blockers = pm_get_blocker();
    print_blockers(&blockers, PM_NUM_MODES);
#endif
}
void print_build_info(void)
{
    printf("Build Information:\n");
    printf("Terra (Build Date: %s, Time of Build: %s)\n", __DATE__, __TIME__);
    printf("THREAD_STACKSIZE_MAIN: %" PRIiLEAST16 " bytes\n", THREAD_STACKSIZE_MAIN);
    printf("EXECUTION_EPOCH_S: %" PRIiLEAST16 " seconds\n", EXECUTION_EPOCH_S);
}