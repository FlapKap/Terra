#include <stdlib.h>
#include <stdio.h>

#include "pb_encode.h"
#include "pb_decode.h"
#include "EndDeviceProtocol.pb.h"
#include "operators.h"



void init_data(Instruction * instr, EndDeviceProtocol_Data *out){
  if(instr->unionCase == 0){
    ExpressionInstruction einstr = instr->data._instruction;
    if (einstr == CONST){
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_CONST;
    } else if (einstr == VAR){
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_VAR;
    } else if (einstr == AND) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_AND;
    } else if (einstr == OR) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_OR;
    } else if (einstr == NOT) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_NOT;
    } else if (einstr == LT) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LT;
    } else if (einstr == GT) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_GT;
    } else if (einstr == EQ) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_EQ;
    } else if (einstr == ADD) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ADD;
    } else if (einstr == SUB) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_SUB;
    } else if (einstr == MUL) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_MUL;
    } else if (einstr == DIV) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_DIV;
    } else if (einstr == MOD) {
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_MOD;
    } 
    // else if(einstr == LOG){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LOG;
    // } else if(einstr == POW){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_POW;
    // } else if(einstr == SQRT){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_SQRT;
    // } else if(einstr == EXP){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_EXP;
    // } else if(einstr == CEIL){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_CEIl;
    // } else if(einstr == FLOOR){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_FLORR;
    // } else if(einstr == ROUND){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ROUND;
    // } else if(einstr == ABS){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ABS;
    // } else if(einstr == LTEG){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LTEQ;
    // } else if(einstr == GTEQ){
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_GTEQ;
    // }
    out->which_data = EndDeviceProtocol_Data_instruction_tag;
  } else if (instr->unionCase == 1){
    out->data._uint32 = instr->data._uint32;
    out->which_data = EndDeviceProtocol_Data__int32_tag;
  } else if (instr->unionCase == 2){
    out->data._int16 = instr->data._int;
    out->which_data = EndDeviceProtocol_Data__int16_tag;
  } else if (instr->unionCase == 3){
    out->data._float = instr->data._float;
    out->which_data = EndDeviceProtocol_Data__float_tag;
  } else if (instr->unionCase == 4){
    out->data._double = instr->data._double;
    out->which_data = EndDeviceProtocol_Data__double_tag;
  } else {
    printf("Unknown unioncase encode output!\n");
  }
}

void init_query_response(QueryResponse * query, EndDeviceProtocol_Output_QueryResponse *out){
  for(int i = 0; i < query->amount; i++){
    EndDeviceProtocol_Data current = EndDeviceProtocol_Data_init_zero;
    init_data(&query->response[i], &current);
    out->response[i] = current;
  }

  out->response_count = query->amount;
  out->id = query->id;
}


void init_output(OutputMessage *_output, EndDeviceProtocol_Output *out) {
  for (int i = 0; i < _output->amount; i++) {
    EndDeviceProtocol_Output_QueryResponse current = EndDeviceProtocol_Output_QueryResponse_init_zero;
    init_query_response(&_output->responses[i], &current);
    out->responses[i] = current;
  }

  out->responses_count = _output->amount;
}

bool encode_output_message(pb_ostream_t *stream, OutputMessage *msg) {
  bool status;

  EndDeviceProtocol_Output output = EndDeviceProtocol_Output_init_zero;
  init_output(msg, &output);
  
  status = pb_encode(stream, EndDeviceProtocol_Output_fields, &output);
  
  if (!status)
  {
    printf("Encoding failed: %s\n", PB_GET_ERROR(stream));
  }

  return status;
}

bool decode_output_message(pb_istream_t *stream, EndDeviceProtocol_Output *output) {
  bool status = pb_decode(stream, EndDeviceProtocol_Output_fields, output);
  
  return status;
}