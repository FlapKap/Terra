#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "protocolTests.h"
#include "testType.h"
#include "environment.h"
#include "encodeInput.h"
#include "encodeOutput.h"
#include "pb_encode.h"
#include "pb_decode.h"

TestResult runProtocolTests(void){
  TestResult res;

  const int numTests = 20;
  res.total = numTests;
  res.passed = 0;
  res.tests = malloc(sizeof(Test) * numTests);

  res.tests[0] = data_is_initialised();
  res.tests[1] = query_is_initialised();
  res.tests[2] = output_is_initialised();
  res.tests[3] = output_messae_gets_encoded();
  res.tests[4] = output_message_gets_decoded();

  res.tests[5] = window_operation_is_initialised();
  res.tests[6] = filter_operation_is_initialised();
  res.tests[7] = map_operation_is_initialised();
  res.tests[8] = operation_is_initialised();
  res.tests[9] = input_query_is_initialised();
  res.tests[10] = message_is_initialised();
  res.tests[11] = message_gets_encoded();
  res.tests[12] = message_gets_decoded();

  res.tests[13] = og_decode_map();
  res.tests[14] = og_test_filter();
  res.tests[15] = og_test_map_filter_msg();
  res.tests[16] = og_window_message();
  res.tests[17] = og_multiple_queries();
  res.tests[18] = og_output_single_response();
  res.tests[19] = og_output_multiple_response();

  return res;
}

// Global encode
Test data_is_initialised(void){
  Test test;
  test.name = "data is initialised";
  test.failed = 0;
  
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
  if(exprData.data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "exprData.data._instruction is not CONST";
    return test;
  }

  if(uintData.data._uint32 != 11) {
    test.failed = 1;
    test.message = "uintData.data._uint is not 11";
    return test;
  }

  if(intData.data._int32 != 11) {
    test.failed = 1;
    test.message = "intData.data._int is not 11";
    return test;
  }

  if(floatData.data._float != 11.0f) {
    test.failed = 1;
    test.message = "floatData.data._float is not 11.0f";
    return test;
  }

  if(doubleData.data._double != 11.0) {
    test.failed = 1;
    test.message = "doubleData.data._double is not 11.0";
    return test;
  }

  return test;
}

// Output message encode test
Test query_is_initialised(void){
  Test test;
  test.name = "query is initialised";
  test.failed = 0;

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

  // Check that query is initialised
  if(queryResponse.response_count != 5) {
    test.failed = 1;
    test.message = "queryResponse.response_count is not 5";
    return test;
  }

  if(queryResponse.response[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "queryResponse.response[0].data.instruction is not CONST";
    return test;
  }

  if(queryResponse.response[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "queryResponse.response[1].data._int32 is not 1";
    return test;
  }

  if(queryResponse.response[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "queryResponse.response[2].data.instruction is not CONST";
    return test;
  }

  if(queryResponse.response[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "queryResponse.response[3].data._int32 is not 1";
    return test;
  }

  if(queryResponse.response[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "queryResponse.response[4].data.instruction is not ADD";
    return test;
  }

  return test;
}


Test output_is_initialised(void){
  Test test;
  test.name = "query is initialised";
  test.failed = 0;

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
  if(outputMessage.responses_count != 1) {
    test.failed = 1;
    test.message = "outputMessage.response_count is not 1";
    return test;
  }

  if(outputMessage.responses[0].response_count != 5) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response_count is not 5";
    return test;
  }

  if(outputMessage.responses[0].response[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[0].data.instruction is not CONST";
    return test;
  }

  if(outputMessage.responses[0].response[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[1].data._int32 is not 1";
    return test;
  }

  if(outputMessage.responses[0].response[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[2].data.instruction is not CONST";
    return test;
  }

  if(outputMessage.responses[0].response[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[3].data._int32 is not 1";
    return test;
  }

  if(outputMessage.responses[0].response[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[4].data.instruction is not ADD";
    return test;
  }

  return test;
}

Test output_messae_gets_encoded(void){
  Test test;
  test.name = "output message gets encoded";
  test.failed = 0;

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

  if(!status) {
    test.failed = 1;
    test.message = "output message could not be encoded";
    return test;
  }

  for(int i = 0; i < message_length; i++) {
    if(buffer[i] != expected[i]) {
      test.failed = 1;
      test.message = "output message was not encoded correctly";
      return test;
    }
  }

  return test;
}

Test output_message_gets_decoded(void){
  Test test;
  test.name = "output message gets decoded";
  test.failed = 0;

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

  if(!status) {
    test.failed = 1;
    test.message = "output message could not be encoded";
    return test;
  }

  // Decode output
  pb_istream_t istream = pb_istream_from_buffer(buffer, message_length);
  EndDeviceProtocol_Output outputMessage = EndDeviceProtocol_Output_init_zero;

  status = decode_output_message(&istream, &outputMessage);

  if(!status) {
    test.failed = 1;
    test.message = "output message could not be decoded";
    return test;
  }

  // Check it has the correct values
  if(outputMessage.responses_count != 1) {
    test.failed = 1;
    test.message = "outputMessage.response_count is not 1";
    return test;
  }

  if(outputMessage.responses[0].response_count != 5) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response_count is not 5";
    return test;
  }

  if(outputMessage.responses[0].response[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[0].data.instruction is not CONST";
    return test;
  }

  if(outputMessage.responses[0].response[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[1].data._int32 is not 1";
    return test;
  }

  if(outputMessage.responses[0].response[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[2].data.instruction is not CONST";
    return test;
  }

  if(outputMessage.responses[0].response[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[3].data._int32 is not 1";
    return test;
  }

  if(outputMessage.responses[0].response[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "outputMessage.response[0].response[4].data.instruction is not ADD";
    return test;
  }

  return test;
}

Test window_operation_is_initialised(void){
  Test test;
  test.name = "window is initialised";
  test.failed = 0;

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
  if(windowOperation.size != 2) {
    test.failed = 1;
    test.message = "windowOperation.size is not 2";
    return test;
  }

  if(windowOperation.sizeType != EndDeviceProtocol_WindowSizeType_TIMEBASED) {
    test.failed = 1;
    test.message = "windowOperation.sizeType is not TIMEBASED";
    return test;
  }

  if(windowOperation.aggregationType != EndDeviceProtocol_WindowAggregationType_SUM) {
    test.failed = 1;
    test.message = "windowOperation.aggregationType is not SUM";
    return test;
  }

  if(windowOperation.startAttribute != 0) {
    test.failed = 1;
    test.message = "windowOperation.startAttribute is not 0";
    return test;
  }

  if(windowOperation.endAttribute != 1) {
    test.failed = 1;
    test.message = "windowOperation.endAttribute is not 1";
    return test;
  }

  if(windowOperation.resultAttribute != 2) {
    test.failed = 1;
    test.message = "windowOperation.resultAttribute is not 2";
    return test;
  }

  if(windowOperation.readAttribute != 3) {
    test.failed = 1;
    test.message = "windowOperation.readAttribute is not 3";
    return test;
  }

  return test;
}

Test filter_operation_is_initialised(void){
  Test test;
  test.name = "filter is initialised";
  test.failed = 0;

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
  if(filterOperation.predicate_count != 5) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions_count is not 5";
    return test;
  }

  if(filterOperation.predicate[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions[0].instruction is not CONST";
    return test;
  }

  if(filterOperation.predicate[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions[1]._int32 is not 1";
    return test;
  }

  if(filterOperation.predicate[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions[2].instruction is not CONST";
    return test;
  }
  
  if(filterOperation.predicate[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions[3]._int32 is not 1";
    return test;
  }

  if(filterOperation.predicate[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "filterOperation.predicate.instructions[4].instruction is not ADD";
    return test;
  }


  return test;
}

Test map_operation_is_initialised(void){
  Test test;
  test.name = "map is initialised";
  test.failed = 0;

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
  if(mapOperation.function_count != 5){
    test.failed = 1;
    test.message = "mapOperation.function.instructions_count is not 5";
    return test;
  }

  if(mapOperation.function[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "mapOperation.function.instructions[0].instruction is not CONST";
    return test;
  }

  if(mapOperation.function[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "mapOperation.function.instructions[1]._int32 is not 1";
    return test;
  }

  if(mapOperation.function[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "mapOperation.function.instructions[2].instruction is not CONST";
    return test;
  }

  if(mapOperation.function[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "mapOperation.function.instructions[3]._int32 is not 1";
    return test;
  }

  if(mapOperation.function[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "mapOperation.function.instructions[4].instruction is not ADD";
    return test;
  }

  return test;
}

Test operation_is_initialised(void){
  Test test;
  test.name = "operation is initialised";
  test.failed = 0;

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
  if(operationOperation.which_operation != EndDeviceProtocol_Operation_map_tag){
    test.failed = 1;
    test.message = "operationOperation.which_operation is not map";
    return test;
  }

  if(operationOperation.operation.map.function_count != 5){
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions_count is not 5";
    return test;
  }

  if(operationOperation.operation.map.function[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions[0].instruction is not CONST";
    return test;
  }

  if(operationOperation.operation.map.function[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions[1]._int32 is not 1";
    return test;
  }

  if(operationOperation.operation.map.function[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions[2].instruction is not CONST";
    return test;
  }

  if(operationOperation.operation.map.function[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions[3]._int32 is not 1";
    return test;
  }

  if(operationOperation.operation.map.function[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "operationOperation.operation.map.function.instructions[4].instruction is not ADD";
    return test;
  }

  return test;
}

Test input_query_is_initialised(void){
  Test test;
  test.name = "query is initialised";
  test.failed = 0;

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
  if(preparedQuery.operations_count != 1){
    test.failed = 1;
    test.message = "preparedQuery.operations_count is not 1";
    return test;
  }

  if(preparedQuery.operations[0].which_operation != EndDeviceProtocol_Operation_map_tag){
    test.failed = 1;
    test.message = "preparedQuery.operations[0].which_operation is not map";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function_count != 5){
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions_count is not 5";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions[0].instruction is not CONST";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions[1]._int32 is not 1";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions[2].instruction is not CONST";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions[3]._int32 is not 1";
    return test;
  }

  if(preparedQuery.operations[0].operation.map.function[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "preparedQuery.operations[0].operation.map.function.instructions[4].instruction is not ADD";
    return test;
  }

  return test;
}

// Encode and decode input message tests
Test message_is_initialised(void){
  Test test;
  test.name = "message is initialised";
  test.failed = 0;

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
  if(preparedMessage.queries_count != 1){
    test.failed = 1;
    test.message = "preparedMessage.queries_count is not 1";
    return test;
  }

  if(preparedMessage.queries[0].operations_count != 1){
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations_count is not 1";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].which_operation != EndDeviceProtocol_Operation_map_tag){
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].which_operation is not map";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function_count != 5){
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions_count is not 5";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function[0].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions[0].instruction is not CONST";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function[1].data._int32 != 1) {
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions[0]._int32 is not 1";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function[2].data.instruction != EndDeviceProtocol_ExpressionInstructions_CONST) {
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions[1].instruction is not CONST";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function[3].data._int32 != 1) {
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions[1]._int32 is not 1";
    return test;
  }

  if(preparedMessage.queries[0].operations[0].operation.map.function[4].data.instruction != EndDeviceProtocol_ExpressionInstructions_ADD) {
    test.failed = 1;
    test.message = "preparedMessage.queries[0].operations[0].operation.map.function.instructions[2].instruction is not ADD";
    return test;
  }

  return test;
}



Test message_gets_encoded(void){
  Test test;
  test.name = "message gets encoded";
  test.failed = 0;

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
  uint8_t expected[] = {0x0a, 0x18, 0x0a, 0x16, 0x0a, 0x14, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x38, 0x02, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x38, 0x02, 0x0a, 0x02, 0x08, 0x08};


  // Check if it was encoded correctly
  if(!status) {
    test.failed = 1;
    test.message = "output message could not be encoded";
    return test;
  }

  for(int i = 0; i < message_length; i++) {
    if(buffer[i] != expected[i]) {
      test.failed = 1;
      test.message = "output message is not encoded correctly";
      return test;
    }
  }

  return test;
}

Test message_gets_decoded(void){
  Test test;
  test.name = "message gets decoded";
  test.failed = 0;

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
  if(!status) {
    test.failed = 1;
    test.message = "output message could not be decoded";
    return test;
  }

  if(decoded.amount != 1) {
    test.failed = 1;
    test.message = "decoded message amount is not 1";
    return test;
  }

  if(decoded.queries[0].amount != 1) {
    test.failed = 1;
    test.message = "decoded message queries[0].amount is not 1";
    return test;
  }

  if(decoded.queries[0].operations[0].unionCase != 0) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].unionCase is not 0";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->attribute != 0) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.attribute is not 0";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->p_size != 5) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.p_size is not 5";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->program[0].data._instruction != CONST) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.program[0].instruction is not CONST";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->program[1].data._int != 1) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.program[1].data._int is not 1";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->program[2].data._instruction != CONST) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.program[2].instruction is not CONST";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->program[3].data._int != 1) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.program[3].data._int is not 1";
    return test;
  }

  if(decoded.queries[0].operations[0].operation.map->expression->program[4].data._instruction != ADD) {
    test.failed = 1;
    test.message = "decoded message queries[0].operations[0].operation.map.expression.program[4].instruction is not ADD";
    return test;
  }

  return test;
}

Test og_decode_map(void){
  Test test;
  test.name = "Decoding map from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a, 0x12, 0x0a, 0x10, 0x0a, 0x0e, 0x0a, 0x02, 0x08, 0x00, 0x0a, 0x02, 0x10, 0x08, 0x0a, 0x02, 0x08, 0x0a, 0x10, 0x01};
  pb_istream_t istream = pb_istream_from_buffer(message, 20);


  Message msg;
  bool status = decode_input_message(&istream, &msg);

  if(status != true) {
    test.failed = 1;
    test.message = "Decoding failed";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First instruction is not CONST";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[1].data._int != 8){
    test.failed = 1;
    test.message = "Second instructions value is not 8";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[2].data._instruction != MUL){
    test.failed = 1;
    test.message = "Third instruction is not MUL";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->attribute != 1){
    test.failed = 1;
    test.message = "Map attribute is not 1";
    return test;
  }

  return test;
}

Test og_test_filter(void){
  Test test;
  test.name = "Decoding filter from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x10,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x02,0x08,0x05};
  pb_istream_t istream = pb_istream_from_buffer(message, 18);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  if(status != true) {
    test.failed = 1;
    test.message = "Decoding failed";
    return test;
  }

  if(msg.queries[0].operations[0].operation.filter->predicate[0].program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First instruction is not CONST";
    return test;
  }

  if(msg.queries[0].operations[0].operation.filter->predicate[0].program[1].data._int != 8){
    test.failed = 1;
    test.message = "Second instructions value is not 8";
    return test;
  }

  if(msg.queries[0].operations[0].operation.filter->predicate[0].program[2].data._instruction != LT){
    test.failed = 1;
    test.message = "Third instruction is not LT";
    return test;
  }

  return test;
}

Test og_test_map_filter_msg(void){
  Test test;
  test.name = "Decoding map and filter from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x20,0x0a,0x0e,0x0a,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x02,0x08,0x0a,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x30,0x64,0x0a,0x02,0x08,0x06};
  pb_istream_t istream = pb_istream_from_buffer(message, 34);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  if(status != true) {
    test.failed = 1;
    test.message = "Decoding failed";
    return test;
  }

  if(msg.queries[0].operations[0].unionCase != 0){
    test.failed = 1;
    test.message = "First operation is not map";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First map instruction is not CONST";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[1].data._int != 8){
    test.failed = 1;
    test.message = "Second map instructions value is not 8";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[2].data._instruction != MUL){
    test.failed = 1;
    test.message = "Third map instruction is not MUL";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->attribute != 0){
    test.failed = 1;
    test.message = "Map attribute is not 0";
    return test;
  }

  if(msg.queries[0].operations[1].unionCase != 1){
    test.failed = 1;
    test.message = "Second operation is not filter";
    return test;
  }

  if(msg.queries[0].operations[1].operation.filter->predicate[0].program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First filter instruction is not CONST";
    return test;
  }

  if(msg.queries[0].operations[1].operation.filter->predicate[0].program[1].data._int != 50){
    test.failed = 1;
    test.message = "Second filter instructions value is not 50";
    return test;
  }

  if(msg.queries[0].operations[1].operation.filter->predicate[0].program[2].data._instruction != GT){
    test.failed = 1;
    test.message = "Third filter instruction is not GT";
    return test;
  }

  return test;
}

Test og_window_message(void){
  Test test;
  test.name = "Decoding window from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x10,0x0a,0x0e,0x1a,0x0c,0x08,0x03,0x10,0x01,0x18,0x04,0x28,0x01,0x30,0x02,0x38,0x03};
  pb_istream_t istream = pb_istream_from_buffer(message, 18);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  if(status != true) {
    test.failed = 1;
    test.message = "Decoding failed";
    return test;
  }

  if(msg.queries[0].operations[0].unionCase != 2){
    test.failed = 1;
    test.message = "First operation is not window";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->aggregationType != COUNT){
    test.failed = 1;
    test.message = "Window aggregation type is not COUNT";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->sizeType != COUNTBASED){
    test.failed = 1;
    test.message = "Window size type is not COUNTBASED";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->size != 3){
    test.failed = 1;
    test.message = "Window size is not 3";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->startAttribute != 0){
    test.failed = 1;
    test.message = "Window start attribute is not 0";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->endAttribute != 1){
    test.failed = 1;
    test.message = "Window end attribute is not 1";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->resultAttribute != 2){
    test.failed = 1;
    test.message = "Window result attribute is not 2";
    return test;
  }

  if(msg.queries[0].operations[0].operation.window->readAttribute != 3){
    test.failed = 1;
    test.message = "Window read attribute is not 3";
    return test;
  }

  return test;

}

Test og_multiple_queries(void){
  Test test;
  test.name = "Decoding multiple queries from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x0c,0x0a,0x0a,0x0a,0x08,0x0a,0x02,0x08,0x00,0x0a,0x02,0x10,0x08,0x0a,0x20,0x0a,0x0e,0x1a,0x0c,0x08,0x03,0x10,0x01,0x18,0x04,0x28,0x01,0x30,0x02,0x38,0x03,0x0a,0x0e,0x12,0x0c,0x0a,0x02,0x08,0x00,0x0a,0x02,0x30,0x64,0x0a,0x02,0x08,0x05};
  pb_istream_t istream = pb_istream_from_buffer(message, 48);

  Message msg;
  bool status = decode_input_message(&istream, &msg);

  if(status != true) {
    test.failed = 1;
    test.message = "Decoding failed";
    return test;
  }

  if(msg.amount != 2){
    test.failed = 1;
    test.message = "Amount of queries is not 2";
    return test;
  }

  if(msg.queries[0].operations[0].unionCase != 0){
    test.failed = 1;
    test.message = "First operation of first query is not map";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->attribute != 0){
    test.failed = 1;
    test.message = "Map attribute is not 0";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First map instruction is not CONST";
    return test;
  }

  if(msg.queries[0].operations[0].operation.map->expression->program[1].data._int != 8){
    test.failed = 1;
    test.message = "Second map instructions value is not 8";
    return test;
  }

  if(msg.queries[1].operations[0].unionCase != 2){
    test.failed = 1;
    test.message = "First operation of second query is not window";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->aggregationType != COUNT){
    test.failed = 1;
    test.message = "Window aggregation type is not COUNT";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->sizeType != COUNTBASED){
    test.failed = 1;
    test.message = "Window size type is not COUNTBASED";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->size != 3){
    test.failed = 1;
    test.message = "Window size is not 3";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->startAttribute != 0){
    test.failed = 1;
    test.message = "Window start attribute is not 0";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->endAttribute != 1){
    test.failed = 1;
    test.message = "Window end attribute is not 1";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->resultAttribute != 2){
    test.failed = 1;
    test.message = "Window result attribute is not 2";
    return test;
  }

  if(msg.queries[1].operations[0].operation.window->readAttribute != 3){
    test.failed = 1;
    test.message = "Window read attribute is not 3";
    return test;
  }

  if(msg.queries[1].operations[1].unionCase != 1){
    test.failed = 1;
    test.message = "Second operation of second query is not filter";
    return test;
  }

  if(msg.queries[1].operations[1].operation.filter->predicate[0].program[0].data._instruction != CONST){
    test.failed = 1;
    test.message = "First filter instruction is not CONST";
    return test;
  }

  if(msg.queries[1].operations[1].operation.filter->predicate[0].program[1].data._int != 50){
    test.failed = 1;
    test.message = "Second filter instructions value is not 50";
    return test;
  }

  if(msg.queries[1].operations[1].operation.filter->predicate[0].program[2].data._instruction != LT){
    test.failed = 1;
    test.message = "Third filter instruction is not LT";
    return test;
  }

  return test;

}

Test og_output_single_response(void){
  Test test;
  test.name = "Encoding a single response from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x16,0x08,0x01,0x12,0x02,0x10,0x48,0x12,0x02,0x10,0x45,0x12,0x02,0x10,0x4c,0x12,0x02,0x10,0x4c,0x12,0x02,0x10,0x4f};

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

  EndDeviceProtocol_Data data[] = {datah, datae, datal, datal2, datao};

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


  if(!status){
    test.failed = 1;
    test.message = "Encoding failed";
    return test;
  }

  // Compare Message
  for(int i = 0; i < message_length; i++){
    if(buffer[i] != message[i]){
      test.failed = 1;
      test.message = "Encoded message does not match expected message";
      return test;
    }
  }
  

  return test;
}

Test og_output_multiple_response(void){
  Test test;
  test.name = "Decoding multiple responses from original testsuite input";
  test.failed = 0;

  uint8_t message[] = {0x0a,0x06,0x08,0x01,0x12,0x02,0x10,0x48,0x0a,0x06,0x08,0x02,0x12,0x02,0x10,0x45,0x0a,0x06,0x08,0x03,0x12,0x02,0x10,0x4c,0x0a,0x06,0x08,0x04,0x12,0x02,0x10,0x4c,0x0a,0x06,0x08,0x05,0x12,0x02,0x10,0x4f};

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

  EndDeviceProtocol_Data data[] = {datah, datae, datal, datal2, datao};

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

  EndDeviceProtocol_Output_QueryResponse responses[] = {response1, response2, response3, response4, response5};

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


  if(!status){
    test.failed = 1;
    test.message = "Encoding failed";
    return test;
  }

  // Compare Message
  for(int i = 0; i < message_length; i++){
    if(buffer[i] != message[i]){
      test.failed = 1;
      test.message = "Encoded message does not match expected message";
      return test;
    }
  }

  return test;
}
