#include <stdlib.h>
#include <stdio.h>

#include "pb_encode.h"
#include "pb_decode.h"
#include "EndDeviceProtocol.pb.h"
#include "operators.h"

void init_data(Instruction *instr, EndDeviceProtocol_Data *out)
{
  switch (instr->unionCase)
  {
  case 0:
  {
    out->which_data = EndDeviceProtocol_Data_instruction_tag;
    switch (instr->data._instruction)
    {
    case CONST:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_CONST;
      break;
    case VAR:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_VAR;
      break;
    case AND:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_AND;
      break;
    case OR:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_OR;
      break;
    case NOT:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_NOT;
      break;
    case LT:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LT;
      break;
    case GT:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_GT;
      break;
    case EQ:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_EQ;
      break;
    case ADD:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ADD;
      break;
    case SUB:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_SUB;
      break;
    case MUL:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_MUL;
      break;
    case DIV:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_DIV;
      break;
    case MOD:
      out->data.instruction = EndDeviceProtocol_ExpressionInstructions_MOD;
      break;
    // case LOG:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LOG;
    //   break;
    // case POW:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_POW;
    //   break;
    // case SQRT:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_SQRT;
    //   break;
    // case EXP:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_EXP;
    //   break;
    // case CEIL:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_CEIl;
    //   break;
    // case FLOOR:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_FLORR;
    //   break;
    // case ROUND:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ROUND;
    //   break;
    // case ABS:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_ABS;
    //   break;
    // case LTEG:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_LTEQ;
    //   break;
    // case GTEQ:
    //   out->data.instruction = EndDeviceProtocol_ExpressionInstructions_GTEQ;
    //   break;
    default:
      // Handle unknown instruction type
      printf("Unknown instruction type!\n");
      break;
    }
    break;
  }
  case 1:
  {
    out->data._uint32 = instr->data._uint32;
    out->which_data = EndDeviceProtocol_Data__int32_tag;
    break;
  }
  case 2:
  {
    out->data._int16 = instr->data._int;
    out->which_data = EndDeviceProtocol_Data__int16_tag;
    break;
  }
  case 3:
  {
    out->data._float = instr->data._float;
    out->which_data = EndDeviceProtocol_Data__float_tag;
    break;
  }
  case 4:
  {
    out->data._double = instr->data._double;
    out->which_data = EndDeviceProtocol_Data__double_tag;
    break;
  }
  default:
  {
    printf("Unknown unioncase encode output!\n");
    break;
  }
  }
}

void init_query_response(QueryResponse *query, EndDeviceProtocol_Output_QueryResponse *out)
{
  for (int i = 0; i < query->amount; i++)
  {
    EndDeviceProtocol_Data current = EndDeviceProtocol_Data_init_zero;
    init_data(&query->response[i], &current);
    out->response[i] = current;
  }

  out->response_count = query->amount;
  out->id = query->id;
}

void init_output(OutputMessage *_output, EndDeviceProtocol_Output *out)
{
  for (int i = 0; i < _output->amount; i++)
  {
    EndDeviceProtocol_Output_QueryResponse current = EndDeviceProtocol_Output_QueryResponse_init_zero;
    init_query_response(&_output->responses[i], &current);
    out->responses[i] = current;
  }

  out->responses_count = _output->amount;
}

bool encode_output_message(pb_ostream_t *stream, OutputMessage *msg)
{
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

bool decode_output_message(pb_istream_t *stream, EndDeviceProtocol_Output *output)
{
  bool status = pb_decode(stream, EndDeviceProtocol_Output_fields, output);

  return status;
}