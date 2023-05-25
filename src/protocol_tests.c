#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "protocol_tests.h"
#include "environment.h"
#include "encodeInput.h"
#include "encodeOutput.h"
#include "pb_encode.h"
#include "pb_decode.h"

// Global encode
static void data_is_initialised(void)
{

    // Create instructions
    Instruction exprInstruction;
    exprInstruction.data._instruction = CONST;
    exprInstruction.unionCase = 0;

    Instruction uintInstruction;
    uintInstruction.data._uint32 = 11;
    uintInstruction.unionCase = 1;

    Instruction intInstr;
    intInstr.data._int = 11;
    intInstr.unionCase = 2;

    Instruction floatInstr;
    floatInstr.data._float = 11.0f;
    floatInstr.unionCase = 3;

    Instruction doubleInstr;
    doubleInstr.data._double = 11.0;
    doubleInstr.unionCase = 4;

    EndDeviceProtocol_Data exprData = EndDeviceProtocol_Data_init_zero;
    EndDeviceProtocol_Data uintData = EndDeviceProtocol_Data_init_zero;
    EndDeviceProtocol_Data intData = EndDeviceProtocol_Data_init_zero;
    EndDeviceProtocol_Data floatData = EndDeviceProtocol_Data_init_zero;
    EndDeviceProtocol_Data doubleData = EndDeviceProtocol_Data_init_zero;

    // Prepare instructions
    init_data(&exprInstruction, &exprData);
    init_data(&uintInstruction, &uintData);
    init_data(&intInstr, &intData);
    init_data(&floatInstr, &floatData);
    init_data(&doubleInstr, &doubleData);

    // Check that data is initialised
    TEST_ASSERT(exprData.data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(uintData.data._uint32, 11);
    TEST_ASSERT_EQUAL_INT(intData.data._int32, 11);
    TEST_ASSERT(floatData.data._float > 10.99f && floatData.data._float < 11.01f);
    TEST_ASSERT(doubleData.data._double > 10.99 && doubleData.data._double < 11.01);
}

// // Output message encode test
static void query_is_initialised(void)
{

    // Create instructions
    Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

    // Create query
    QueryResponse query;
    query.id = 1;
    query.amount = 5;
    query.response = p;

    // Prepare query
    EndDeviceProtocol_Output_QueryResponse queryResponse = EndDeviceProtocol_Output_QueryResponse_init_zero;
    init_query_response(&query, &queryResponse);

    TEST_ASSERT_EQUAL_INT(5, queryResponse.response_count);
    TEST_ASSERT(queryResponse.response[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, queryResponse.response[1].data._int32);
    TEST_ASSERT(queryResponse.response[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, queryResponse.response[3].data._int32);
    TEST_ASSERT(queryResponse.response[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

static void output_is_initialised(void)
{

    // Create instructions
    Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

    // Create query
    QueryResponse query;
    query.id = 1;
    query.amount = 5;
    query.response = p;

    // Create Output
    OutputMessage output;
    output.amount = 1;
    output.responses = &query;

    // Prepare output
    EndDeviceProtocol_Output outputMessage = EndDeviceProtocol_Output_init_zero;
    init_output(&output, &outputMessage);

    // Check that output is initialised
    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses_count);
    TEST_ASSERT_EQUAL_INT(5, outputMessage.responses[0].response_count);
    TEST_ASSERT(outputMessage.responses[0].response[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses[0].response[1].data._int32);
    TEST_ASSERT(outputMessage.responses[0].response[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses[0].response[3].data._int32);
    TEST_ASSERT(outputMessage.responses[0].response[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

static void output_message_gets_encoded(void)
{

    // Create instructions
    Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

    // Create query
    QueryResponse query;
    query.id = 1;
    query.amount = 5;
    query.response = p;

    // Create Output
    OutputMessage output;
    output.amount = 1;
    output.responses = &query;

    // Encode output
    uint8_t buffer[1024];
    int message_length;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = encode_output_message(&stream, &output);

    message_length = stream.bytes_written;

    // Expected bytes
    uint8_t expected[] = {0x0A, 0x16, 0x08, 0x01, 0x12, 0x02, 0x08, 0x00, 0x12, 0x02, 0x38, 0x02, 0x12, 0x02, 0x08, 0x00, 0x12, 0x02, 0x38, 0x02, 0x12, 0x02, 0x08, 0x08};

    TEST_ASSERT(status);

    for (int i = 0; i < message_length; i++)
    {
        TEST_ASSERT_EQUAL_INT(expected[i], buffer[i]);
    }
}

static void output_message_gets_decoded(void)
{

    // Create instructions
    Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

    // Create query
    QueryResponse query;
    query.id = 1;
    query.amount = 5;
    query.response = p;

    // Create Output
    OutputMessage output;
    output.amount = 1;
    output.responses = &query;

    // Encode output
    uint8_t buffer[1024];
    int message_length;
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = encode_output_message(&ostream, &output);

    message_length = ostream.bytes_written;
    TEST_ASSERT(status);

    // Decode output
    pb_istream_t istream = pb_istream_from_buffer(buffer, message_length);
    EndDeviceProtocol_Output outputMessage = EndDeviceProtocol_Output_init_zero;

    status = decode_output_message(&istream, &outputMessage);
    TEST_ASSERT(status);

    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses_count);
    TEST_ASSERT_EQUAL_INT(5, outputMessage.responses[0].response_count);
    TEST_ASSERT(outputMessage.responses[0].response[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses[0].response[1].data._int32);
    TEST_ASSERT(outputMessage.responses[0].response[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, outputMessage.responses[0].response[3].data._int32);
    TEST_ASSERT(outputMessage.responses[0].response[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

static void window_operation_is_initialised(void)
{

    // Create window
    Window window;
    window.size = 2;
    window.sizeType = TIMEBASED;
    window.aggregationType = SUM;
    window.startAttribute = 0;
    window.endAttribute = 1;
    window.resultAttribute = 2;
    window.readAttribute = 3;

    // Prepare window
    EndDeviceProtocol_WindowOperation windowOperation = EndDeviceProtocol_WindowOperation_init_zero;
    init_window_operation(window, &windowOperation);

    // Check it has the correct values
    TEST_ASSERT_EQUAL_INT(2, windowOperation.size);
    TEST_ASSERT(windowOperation.sizeType == EndDeviceProtocol_WindowSizeType_TIMEBASED);
    TEST_ASSERT(windowOperation.aggregationType == EndDeviceProtocol_WindowAggregationType_SUM);

    TEST_ASSERT_EQUAL_INT(0, windowOperation.startAttribute);
    TEST_ASSERT_EQUAL_INT(1, windowOperation.endAttribute);
    TEST_ASSERT_EQUAL_INT(2, windowOperation.resultAttribute);
    TEST_ASSERT_EQUAL_INT(3, windowOperation.readAttribute);
}

static void filter_operation_is_initialised(void)
{

    // Create instructions
    Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

    // Create Expression
    Expression expression;
    expression.program = p;
    expression.p_size = 5;

    // Create filter
    Filter filter;
    filter.predicate = &expression;

    // Prepare filter
    EndDeviceProtocol_FilterOperation filterOperation = EndDeviceProtocol_FilterOperation_init_zero;
    init_filter_operation(filter, &filterOperation);

    // Check it has the correct values
    TEST_ASSERT_EQUAL_INT(5, filterOperation.predicate_count);
    TEST_ASSERT(filterOperation.predicate[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, filterOperation.predicate[1].data._int32);
    TEST_ASSERT(filterOperation.predicate[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
    TEST_ASSERT_EQUAL_INT(1, filterOperation.predicate[3].data._int32);
    TEST_ASSERT(filterOperation.predicate[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

static void map_operation_is_initialised(void){
  // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Prepare map
  EndDeviceProtocol_MapOperation mapOperation = EndDeviceProtocol_MapOperation_init_zero;
  init_map_operation(map, &mapOperation);

  // Check it has the correct values
  TEST_ASSERT_EQUAL_INT(5, mapOperation.function_count);

  TEST_ASSERT(mapOperation.function[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(mapOperation.function[1].data._int32, 1);
  TEST_ASSERT(mapOperation.function[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(mapOperation.function[3].data._int32, 1);
  TEST_ASSERT(mapOperation.function[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

void operation_is_initialised(void){

  // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Create operation
  Operation operation;
  operation.operation.map = &map;
  operation.unionCase = 0;

  // Prepare operation
  EndDeviceProtocol_Operation operationOperation = EndDeviceProtocol_Operation_init_zero;
  init_operation(operation, &operationOperation);

  // Check it has the correct values
  TEST_ASSERT(operationOperation.which_operation == EndDeviceProtocol_Operation_map_tag);
  TEST_ASSERT_EQUAL_INT(5, operationOperation.operation.map.function_count);
  TEST_ASSERT(operationOperation.operation.map.function[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(1,operationOperation.operation.map.function[1].data._int32);
  TEST_ASSERT(operationOperation.operation.map.function[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(1, operationOperation.operation.map.function[3].data._int32);
  TEST_ASSERT(operationOperation.operation.map.function[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

void input_query_is_initialised(void){
  // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Create operation
  Operation operation;
  operation.operation.map = &map;
  operation.unionCase = 0;

  // Create query
  Query query;
  query.operations = &operation;
  query.amount = 1;

  // Prepare query
  EndDeviceProtocol_Query preparedQuery = EndDeviceProtocol_Query_init_zero;
  init_query(query, &preparedQuery);

  // Check it has the correct values
  TEST_ASSERT_EQUAL_INT(1,preparedQuery.operations_count);
  TEST_ASSERT(preparedQuery.operations[0].which_operation == EndDeviceProtocol_Operation_map_tag);
  TEST_ASSERT_EQUAL_INT(5,preparedQuery.operations[0].operation.map.function_count);
  TEST_ASSERT(preparedQuery.operations[0].operation.map.function[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);

  TEST_ASSERT_EQUAL_INT(1,preparedQuery.operations[0].operation.map.function[1].data._int32);

  TEST_ASSERT(preparedQuery.operations[0].operation.map.function[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(1, preparedQuery.operations[0].operation.map.function[3].data._int32);
  TEST_ASSERT(preparedQuery.operations[0].operation.map.function[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

// Encode and decode input message tests
void message_is_initialised(void){
  // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Create operation
  Operation operation;
  operation.operation.map = &map;
  operation.unionCase = 0;

  // Create query
  Query query;
  query.operations = &operation;
  query.amount = 1;

  // Create message
  Message message;
  message.queries = &query;
  message.amount = 1;

  // Prepare message
  EndDeviceProtocol_Message preparedMessage = EndDeviceProtocol_Message_init_zero;
  init_message(message, &preparedMessage);

  // Check it has the correct values
  TEST_ASSERT_EQUAL_INT(1,preparedMessage.queries_count);
  TEST_ASSERT_EQUAL_INT(1,preparedMessage.queries[0].operations_count);
  TEST_ASSERT(preparedMessage.queries[0].operations[0].which_operation == EndDeviceProtocol_Operation_map_tag);
  TEST_ASSERT_EQUAL_INT(5,preparedMessage.queries[0].operations[0].operation.map.function_count);
  TEST_ASSERT(preparedMessage.queries[0].operations[0].operation.map.function[0].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(1, preparedMessage.queries[0].operations[0].operation.map.function[1].data._int32);
  TEST_ASSERT(preparedMessage.queries[0].operations[0].operation.map.function[2].data.instruction == EndDeviceProtocol_ExpressionInstructions_CONST);
  TEST_ASSERT_EQUAL_INT(1,preparedMessage.queries[0].operations[0].operation.map.function[3].data._int32);
  TEST_ASSERT(preparedMessage.queries[0].operations[0].operation.map.function[4].data.instruction == EndDeviceProtocol_ExpressionInstructions_ADD);
}

void message_gets_encoded(void){

  // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Create operation
  Operation operation;
  operation.operation.map = &map;
  operation.unionCase = 0;

  // Create query
  Query query;
  query.operations = &operation;
  query.amount = 1;

  // Create message
  Message message;
  message.queries = &query;
  message.amount = 1;

  // Create stream
  uint8_t buffer[1024];
  int message_length;
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

  // Encode message
  bool status = encode_message(&stream, message);
  message_length = stream.bytes_written;

  // Expected output
  const uint8_t expected[] = {0x0a, 0x18, 0x0a, 0x16, 0x0a, 0x14, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x38, 0x02, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x38, 0x02, 0x0a, 0x02, 0x08, 0x08};

  // Check if it was encoded correctly
  TEST_ASSERT(status);

  for(int i = 0; i < message_length; i++) {
    TEST_ASSERT(buffer[i] == expected[i]);
  }
}

void message_gets_decoded(void){

    // Create instructions
  Instruction p[5] = {{{CONST}, 0}, {.data._int = 1, 2}, {{CONST}, 0}, {.data._int = 1, 2}, {{ADD}, 0}};

  // Create Expression
  Expression expression;
  expression.program = p;
  expression.p_size = 5;

  // Create map
  Map map;
  map.expression = &expression;
  map.attribute = 0;

  // Create operation
  Operation operation;
  operation.operation.map = &map;
  operation.unionCase = 0;

  // Create query
  Query query;
  query.operations = &operation;
  query.amount = 1;

  // Create message
  Message message;
  message.queries = &query;
  message.amount = 1;

  // Create outputstream
  uint8_t buffer[1024];
  int message_length;
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

  // Encode message
  encode_message(&stream, message);
  message_length = stream.bytes_written;

  // Create inputstream
  pb_istream_t istream = pb_istream_from_buffer(buffer, message_length);

  // Decode message
  Message decoded;
  bool status = decode_input_message(&istream, &decoded);

  // Check if it was decoded correctly
  TEST_ASSERT(status);
  TEST_ASSERT_EQUAL_INT(1,decoded.amount);
  TEST_ASSERT_EQUAL_INT(1,decoded.queries[0].amount);
  TEST_ASSERT_EQUAL_INT(0,decoded.queries[0].operations[0].unionCase);
  TEST_ASSERT_EQUAL_INT(0,decoded.queries[0].operations[0].operation.map->attribute);
  TEST_ASSERT_EQUAL_INT(5,decoded.queries[0].operations[0].operation.map->expression->p_size);
  TEST_ASSERT(decoded.queries[0].operations[0].operation.map->expression->program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(1,decoded.queries[0].operations[0].operation.map->expression->program[1].data._int);
  TEST_ASSERT(decoded.queries[0].operations[0].operation.map->expression->program[2].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(1,decoded.queries[0].operations[0].operation.map->expression->program[3].data._int);
  TEST_ASSERT(decoded.queries[0].operations[0].operation.map->expression->program[4].data._instruction == ADD);
}

void og_decode_map(void){

  uint8_t message[] = {0x0a, 0x12, 0x0a, 0x10, 0x0a, 0x0e, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x10, 0x08, 0x0a, 0x02, 0x08, 0x0a, 0x10, 0x01};
  pb_istream_t istream = pb_istream_from_buffer(message, 20);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  TEST_ASSERT(status);
  TEST_ASSERT(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(8,msg.queries[0].operations[0].operation.map->expression->program[1].data._int);
  TEST_ASSERT(msg.queries[0].operations[0].operation.map->expression->program[2].data._instruction == MUL);
  TEST_ASSERT_EQUAL_INT(1,msg.queries[0].operations[0].operation.map->attribute);
}

void og_test_filter(void){

  uint8_t message[] = {0x0a,0x10,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x02,0x08,0x05};
  pb_istream_t istream = pb_istream_from_buffer(message, 18);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  TEST_ASSERT(status);
  TEST_ASSERT(msg.queries[0].operations[0].operation.filter->predicate[0].program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(8,msg.queries[0].operations[0].operation.filter->predicate[0].program[1].data._int);
  TEST_ASSERT(msg.queries[0].operations[0].operation.filter->predicate[0].program[2].data._instruction == LT);
}

void og_test_map_filter_msg(void){

  uint8_t message[] = {0x0a,0x20,0x0a,0x0e,0x0a,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x02,0x08,0x0a,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x30,0x64,0x0a,0x02,0x08,0x06};
  pb_istream_t istream = pb_istream_from_buffer(message, 34);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  TEST_ASSERT(status);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[0].operations[0].unionCase);
  TEST_ASSERT(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(8,msg.queries[0].operations[0].operation.map->expression->program[1].data._int);
  TEST_ASSERT(msg.queries[0].operations[0].operation.map->expression->program[2].data._instruction == MUL);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[0].operations[0].operation.map->attribute);
  TEST_ASSERT_EQUAL_INT(1,msg.queries[0].operations[1].unionCase);
  TEST_ASSERT(msg.queries[0].operations[1].operation.filter->predicate[0].program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(50,msg.queries[0].operations[1].operation.filter->predicate[0].program[1].data._int);
  TEST_ASSERT(msg.queries[0].operations[1].operation.filter->predicate[0].program[2].data._instruction == GT);
}

void og_window_message(void){

  uint8_t message[] = {0x0a,0x10,0x0a,0x0e,0x1a,0x0c,0x08,0x03,0x10,0x01,0x18,0x04,0x28,0x01,0x30,0x02,0x38,0x03};
  pb_istream_t istream = pb_istream_from_buffer(message, 18);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  TEST_ASSERT(status);
  TEST_ASSERT_EQUAL_INT(2,msg.queries[0].operations[0].unionCase);
  TEST_ASSERT(msg.queries[0].operations[0].operation.window->aggregationType == COUNT);
  TEST_ASSERT(msg.queries[0].operations[0].operation.window->sizeType == COUNTBASED);
  TEST_ASSERT_EQUAL_INT(3,msg.queries[0].operations[0].operation.window->size);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[0].operations[0].operation.window->startAttribute);
  TEST_ASSERT_EQUAL_INT(1,msg.queries[0].operations[0].operation.window->endAttribute);
  TEST_ASSERT_EQUAL_INT(2,msg.queries[0].operations[0].operation.window->resultAttribute);
  TEST_ASSERT_EQUAL_INT(3,msg.queries[0].operations[0].operation.window->readAttribute);
}

void og_multiple_queries(void){
  uint8_t message[] = {0x0a,0x0c,0x0a,0x0a,0x0a,0x08,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x20,0x0a,0x0e,0x1a,0x0c,0x08,0x03,0x10,0x01,0x18,0x04,0x28,0x01,0x30,0x02,0x38,0x03,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x30,0x64,0x0a,0x02,0x08,0x05};
  pb_istream_t istream = pb_istream_from_buffer(message, 48);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  TEST_ASSERT(status);
  TEST_ASSERT_EQUAL_INT(2,msg.amount);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[0].operations[0].unionCase);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[0].operations[0].operation.map->attribute);
  TEST_ASSERT(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(8,msg.queries[0].operations[0].operation.map->expression->program[1].data._int);
  TEST_ASSERT_EQUAL_INT(2,msg.queries[1].operations[0].unionCase);
  TEST_ASSERT(msg.queries[1].operations[0].operation.window->aggregationType == COUNT);
  TEST_ASSERT(msg.queries[1].operations[0].operation.window->sizeType == COUNTBASED);
  TEST_ASSERT_EQUAL_INT(3,msg.queries[1].operations[0].operation.window->size);
  TEST_ASSERT_EQUAL_INT(0,msg.queries[1].operations[0].operation.window->startAttribute);
  TEST_ASSERT_EQUAL_INT(1,msg.queries[1].operations[0].operation.window->endAttribute);
  TEST_ASSERT_EQUAL_INT(2,msg.queries[1].operations[0].operation.window->resultAttribute);
  TEST_ASSERT_EQUAL_INT(3,msg.queries[1].operations[0].operation.window->readAttribute);
  TEST_ASSERT_EQUAL_INT(1,msg.queries[1].operations[1].unionCase);
  TEST_ASSERT(msg.queries[1].operations[1].operation.filter->predicate[0].program[0].data._instruction == CONST);
  TEST_ASSERT_EQUAL_INT(50,msg.queries[1].operations[1].operation.filter->predicate[0].program[1].data._int);
  TEST_ASSERT(msg.queries[1].operations[1].operation.filter->predicate[0].program[2].data._instruction == LT);

}

void og_output_single_response(void){

  const uint8_t message[] = {0x0a,0x16,0x08,0x01,0x12,0x02,0x10,0x48,0x12,0x02,0x10,0x45,0x12,0x02,0x10,0x4c,0x12,0x02,0x10,0x4c,0x12,0x02,0x10,0x4f};

  // Create Message
  EndDeviceProtocol_Data datah;
  datah.data._uint8 = 0x48;
  datah.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datae;
  datae.data._uint8 = 0x45;
  datae.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datal;
  datal.data._uint8 = 0x4c;
  datal.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datal2;
  datal2.data._uint8 = 0x4c;
  datal2.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datao;
  datao.data._uint8 = 0x4f;
  datao.which_data = EndDeviceProtocol_Data__uint8_tag;

  const EndDeviceProtocol_Data data[] = {datah, datae, datal, datal2, datao};

  EndDeviceProtocol_Output_QueryResponse response;
  for(int i = 0; i < 5; i++){
    response.response[i] = data[i];
  }
  response.id = 1;
  response.response_count = 5;

  EndDeviceProtocol_Output output;
  output.responses[0] = response;
  output.responses_count = 1;

  // Encode Message
  uint8_t buffer[1024];
  int message_length;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  bool status = pb_encode(&stream, EndDeviceProtocol_Output_fields, &output);
  message_length = stream.bytes_written;

  TEST_ASSERT(status);
  // Compare Message
  for(int i = 0; i < message_length; i++){
    TEST_ASSERT(buffer[i] == message[i]);
  }
}

void og_output_multiple_response(void){

  const uint8_t message[] = {0x0a,0x06,0x08,0x01,0x12,0x02,0x10,0x48,0x0a,0x06,0x08,0x02,0x12,0x02,0x10,0x45,0x0a,0x06,0x08,0x03,0x12,0x02,0x10,0x4c,0x0a,0x06,0x08,0x04,0x12,0x02,0x10,0x4c,0x0a,0x06,0x08,0x05,0x12,0x02,0x10,0x4f};

  // Create Message
  EndDeviceProtocol_Data datah;
  datah.data._uint8 = 0x48;
  datah.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datae;
  datae.data._uint8 = 0x45;
  datae.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datal;
  datal.data._uint8 = 0x4c;
  datal.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datal2;
  datal2.data._uint8 = 0x4c;
  datal2.which_data = EndDeviceProtocol_Data__uint8_tag;

  EndDeviceProtocol_Data datao;
  datao.data._uint8 = 0x4f;
  datao.which_data = EndDeviceProtocol_Data__uint8_tag;

  const EndDeviceProtocol_Data data[] = {datah, datae, datal, datal2, datao};

  EndDeviceProtocol_Output_QueryResponse response1;
  response1.response[0] = data[0];
  response1.id = 1;
  response1.response_count = 1;

  EndDeviceProtocol_Output_QueryResponse response2;
  response2.response[0] = data[1];
  response2.id = 2;
  response2.response_count = 1;

  EndDeviceProtocol_Output_QueryResponse response3;
  response3.response[0] = data[2];
  response3.id = 3;
  response3.response_count = 1;

  EndDeviceProtocol_Output_QueryResponse response4;
  response4.response[0] = data[3];
  response4.id = 4;
  response4.response_count = 1;

  EndDeviceProtocol_Output_QueryResponse response5;
  response5.response[0] = data[4];
  response5.id = 5;
  response5.response_count = 1;

  const EndDeviceProtocol_Output_QueryResponse responses[] = {response1, response2, response3, response4, response5};

  EndDeviceProtocol_Output output;
  for(int i = 0; i < 5; i++){
    output.responses[i] = responses[i];
  }
  output.responses_count = 5;

  // Encode Message
  uint8_t buffer[1024];
  int message_length;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  bool status = pb_encode(&stream, EndDeviceProtocol_Output_fields, &output);
  message_length = stream.bytes_written;

  TEST_ASSERT(status);

  // Compare Message
  for(int i = 0; i < message_length; i++){
    TEST_ASSERT(buffer[i] == message[i]);
  }
}

TestRef tests_protocol(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture(data_is_initialised),
        new_TestFixture(query_is_initialised),
        new_TestFixture(output_is_initialised),
        new_TestFixture(output_message_gets_encoded),
        new_TestFixture(output_message_gets_decoded),
        new_TestFixture(window_operation_is_initialised),
        new_TestFixture(filter_operation_is_initialised),
        new_TestFixture(map_operation_is_initialised),
        new_TestFixture(operation_is_initialised),
        new_TestFixture(input_query_is_initialised),
        new_TestFixture(message_is_initialised),
        new_TestFixture(message_gets_encoded),
        new_TestFixture(message_gets_decoded),
        new_TestFixture(og_decode_map),
        new_TestFixture(og_test_filter),
        new_TestFixture(og_test_map_filter_msg),
        new_TestFixture(og_window_message),
        new_TestFixture(og_multiple_queries),
        new_TestFixture(og_output_single_response),
        new_TestFixture(og_output_multiple_response)
    };
    EMB_UNIT_TESTCALLER(protocol_tests, NULL, NULL, fixtures);
    return (TestRef)&protocol_tests;
}