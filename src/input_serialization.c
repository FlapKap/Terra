

#include "operators.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "input_serialization.h"
#include "output_serialization.h"
#include "EndDeviceProtocol.pb.h"

void init_map_operation(Map map, EndDeviceProtocol_MapOperation *out){
  for(int i = 0; i < map.expression->p_size; i++){
    EndDeviceProtocol_Data current = EndDeviceProtocol_Data_init_zero;
    init_data(&map.expression->program[i], &current);
    out->function.instructions[i] = current;
  }
  
  out->attribute = map.attribute;
  out->function.instructions_count = map.expression->p_size;
}

void init_filter_operation(Filter filter, EndDeviceProtocol_FilterOperation *out) {
  for (int i = 0; i < filter.predicate->p_size; i++) {
    EndDeviceProtocol_Data current = EndDeviceProtocol_Data_init_zero;
    init_data(&filter.predicate->program[i], &current);
    out->predicate.instructions[i] = current;
  }
  out->predicate.instructions_count = filter.predicate->p_size;
}

void init_window_operation(Window window, EndDeviceProtocol_WindowOperation *out){
  out->size = window.size;
    switch (window.sizeType) {
        case TIMEBASED:
            out->sizeType = EndDeviceProtocol_WindowSizeType_TIMEBASED;
            break;
        case COUNTBASED:
            out->sizeType = EndDeviceProtocol_WindowSizeType_COUNTBASED;
            break;
        default:
            printf("unknown window sizetype!\n");
    }

  switch (window.aggregationType) {
    case MIN:
        out->aggregationType = EndDeviceProtocol_WindowAggregationType_MIN;
        break;
    case MAX:
        out->aggregationType = EndDeviceProtocol_WindowAggregationType_MAX;
        break;
    case SUM:
        out->aggregationType = EndDeviceProtocol_WindowAggregationType_SUM;
        break;
    case AVG:
        out->aggregationType = EndDeviceProtocol_WindowAggregationType_AVG;
        break;
    case COUNT:
        out->aggregationType = EndDeviceProtocol_WindowAggregationType_COUNT;
        break;
    default:
        printf("unknown window aggregation type!\n");
  }
  
  out->startAttribute = window.startAttribute;
  out->endAttribute = window.endAttribute;
  out->resultAttribute = window.resultAttribute;
  out->readAttribute = window.readAttribute;
}

void init_operation(Operation op, EndDeviceProtocol_Operation *out){
  // wont these initialisations be created on the stack?
  switch (op.unionCase) {
    case 0: {
      EndDeviceProtocol_MapOperation current = EndDeviceProtocol_MapOperation_init_zero;
      init_map_operation(*op.operation.map, &current);
      out->operation.map = current;
      out->which_operation = EndDeviceProtocol_Operation_map_tag;
      break;
    }
    case 1: {
      EndDeviceProtocol_FilterOperation current = EndDeviceProtocol_FilterOperation_init_zero;
      init_filter_operation(*op.operation.filter, &current);
      out->operation.filter = current;
      out->which_operation = EndDeviceProtocol_Operation_filter_tag;
      break;
    }

    case 2: {
      EndDeviceProtocol_WindowOperation current = EndDeviceProtocol_WindowOperation_init_zero;
      init_window_operation(*op.operation.window, &current);
      out->operation.window = current;
      out->which_operation = EndDeviceProtocol_Operation_window_tag;
      break;
    }
    default:
      printf("Unknown operation type!\n");
  }
}

 void init_query(Query query, EndDeviceProtocol_Query *out){
  for(int i = 0; i < query.amount; i++){
    EndDeviceProtocol_Operation current = EndDeviceProtocol_Operation_init_zero;
    init_operation(query.operations[i], &current);
    out->operations[i] = current;
  }

  out->operations_count = query.amount;
}

void init_message(Message msg, EndDeviceProtocol_Message *out) {
  for (int i = 0; i < msg.amount; i++) {
    EndDeviceProtocol_Query current = EndDeviceProtocol_Query_init_zero;
    init_query(msg.queries[i], &current);
    out->queries[i] = current;
  }

  out->queries_count = msg.amount;
}

bool encode_message(pb_ostream_t *stream, Message message){
  bool status;

  EndDeviceProtocol_Message msg = EndDeviceProtocol_Message_init_zero;
  init_message(message, &msg);
  
  status = pb_encode(stream, EndDeviceProtocol_Message_fields, &msg);
  
  if (!status)
  {
    printf("Encoding failed: %s\n", PB_GET_ERROR(stream));
  }

  return status;
}

bool decode_input_message(pb_istream_t *stream, Message *out) {
  EndDeviceProtocol_Message message = EndDeviceProtocol_Message_init_zero;
  
  bool status = pb_decode(stream, EndDeviceProtocol_Message_fields, &message);

  if (!status) {
    printf("Decoding failed: %s\n", PB_GET_ERROR(stream));
    return false;
  }

  decoded_input_to_message(message, out);

  return status;
}

void decoded_input_to_message(EndDeviceProtocol_Message decoded, Message *out){
  out->amount = decoded.queries_count;

  Query *queries = (Query *)calloc(sizeof(Query), decoded.queries_count);

  for(int i = 0; i < decoded.queries_count; i++){
    Query current;
    decoded_input_to_query(decoded.queries[i], &current);
    queries[i] = current;
  }

  out->queries = queries;
}

void decoded_input_to_query(EndDeviceProtocol_Query decoded, Query *out){
  out->amount = decoded.operations_count;

  Operation *operations = (Operation *)calloc(sizeof(Operation),decoded.operations_count);

  for(int i = 0; i < decoded.operations_count; i++){
    Operation current;
    decoded_input_to_operation(decoded.operations[i], &current);
    operations[i] = current;
  }

  out->operations = operations;
}

void decoded_input_to_operation(EndDeviceProtocol_Operation decoded, Operation *out){
  if(decoded.which_operation == EndDeviceProtocol_Operation_map_tag){
    Map *current = (Map *)calloc(sizeof(Map),1);
    decoded_input_to_map_operation(decoded.operation.map, current);
    out->operation.map = current;
    out->unionCase = 0;
  } else if(decoded.which_operation == EndDeviceProtocol_Operation_filter_tag){
    Filter *current = (Filter *)calloc(sizeof(Filter),1);
    decoded_input_to_filter_operation(decoded.operation.filter, current);
    out->operation.filter = current;
    out->unionCase = 1;
  } else if(decoded.which_operation == EndDeviceProtocol_Operation_window_tag){
    Window *current = (Window *)calloc(sizeof(Window),1);
    decoded_input_to_window_operation(decoded.operation.window, current);
    out->operation.window = current;
    out->unionCase = 2;
  } else {
    printf("Unkown operation type!\n");
  }
}

void decoded_input_to_window_operation(EndDeviceProtocol_WindowOperation input, Window *out) {
  out->size = input.size;

  switch (input.sizeType) {
    case EndDeviceProtocol_WindowSizeType_COUNTBASED:
      out->sizeType = COUNTBASED;
      break;
    case EndDeviceProtocol_WindowSizeType_TIMEBASED:
      out->sizeType = TIMEBASED;
      break;
    default:
      printf("Unknown window size type!\n");
      break;
  }

  switch (input.aggregationType) {
    case EndDeviceProtocol_WindowAggregationType_MIN:
      out->aggregationType = MIN;
      break;
    case EndDeviceProtocol_WindowAggregationType_MAX:
      out->aggregationType = MAX;
      break;
    case EndDeviceProtocol_WindowAggregationType_SUM:
      out->aggregationType = SUM;
      break;
    case EndDeviceProtocol_WindowAggregationType_AVG:
      out->aggregationType = AVG;
      break;
    case EndDeviceProtocol_WindowAggregationType_COUNT:
      out->aggregationType = COUNT;
      break;
    default:
      printf("Unknown window aggregation type!\n");
      break;
  }

  out->startAttribute = input.startAttribute;
  out->endAttribute = input.endAttribute;
  out->resultAttribute = input.resultAttribute;
  out->readAttribute = input.readAttribute;
}

void decoded_input_to_filter_operation(EndDeviceProtocol_FilterOperation decoded, Filter *out){
  Expression *submessage = (Expression *)calloc(sizeof(Expression),1);
  submessage->p_size = decoded.predicate.instructions_count;
  
  Instruction *instructions = (Instruction *)calloc(sizeof(Instruction),decoded.predicate.instructions_count);

  for(int i = 0; i < decoded.predicate.instructions_count; i++){
    Instruction *current = (Instruction *)calloc(sizeof(Instruction),1);
    decoded_input_to_instruction(decoded.predicate.instructions[i], current);
    instructions[i] = *current;
  }

  submessage->program = instructions;

  out->predicate = submessage;
}

void decoded_input_to_map_operation(EndDeviceProtocol_MapOperation decoded, Map *out){
  out->attribute = decoded.attribute;

  Expression *submessage = (Expression *)calloc(sizeof(Expression),1);
  submessage->p_size = decoded.function.instructions_count;
  
  Instruction *instructions = (Instruction *)calloc(sizeof(Instruction), decoded.function.instructions_count);
  
  for(int i = 0; i < decoded.function.instructions_count; i++){
    Instruction *current = (Instruction *)calloc(sizeof(Instruction),1);
    decoded_input_to_instruction(decoded.function.instructions[i], current);
    instructions[i] = *current;
  }

  submessage->program = instructions;

  out->expression = submessage;
}

void decoded_input_to_instruction(EndDeviceProtocol_Data input, Instruction *out){
  //TODO: split this up to make it shorter
  switch (input.which_data) {
    case EndDeviceProtocol_Data_instruction_tag:
      switch (input.data.instruction) {
        case EndDeviceProtocol_ExpressionInstructions_CONST:
          out->data._instruction = CONST;
          break;
        case EndDeviceProtocol_ExpressionInstructions_VAR:
          out->data._instruction = VAR;
          break;
        case EndDeviceProtocol_ExpressionInstructions_AND:
          out->data._instruction = AND;
          break;
        case EndDeviceProtocol_ExpressionInstructions_OR:
          out->data._instruction = OR;
          break;
        case EndDeviceProtocol_ExpressionInstructions_NOT:
          out->data._instruction = NOT;
          break;
        case EndDeviceProtocol_ExpressionInstructions_LT:
          out->data._instruction = LT;
          break;
        case EndDeviceProtocol_ExpressionInstructions_GT:
          out->data._instruction = GT;
          break;
        case EndDeviceProtocol_ExpressionInstructions_EQ:
          out->data._instruction = EQ;
          break;
        case EndDeviceProtocol_ExpressionInstructions_ADD:
          out->data._instruction = ADD;
          break;
        case EndDeviceProtocol_ExpressionInstructions_SUB:
          out->data._instruction = SUB;
          break;
        case EndDeviceProtocol_ExpressionInstructions_MUL:
          out->data._instruction = MUL;
          break;
        case EndDeviceProtocol_ExpressionInstructions_DIV:
          out->data._instruction = DIV;
          break;
        case EndDeviceProtocol_ExpressionInstructions_MOD:
          out->data._instruction = MOD;
          break;
        default:
          // Unknown instruction type
          printf("Unknown instruction type!\n");
          break;
      }
      out->unionCase = 0;
      break;

    case EndDeviceProtocol_Data__uint8_tag:
      out->data._uint32 = input.data._uint8;
      out->unionCase = 1;
      break;
    case EndDeviceProtocol_Data__uint16_tag:
      out->data._uint32 = input.data._uint16;
      out->unionCase = 1;
      break;
    case EndDeviceProtocol_Data__uint32_tag:
      out->data._uint32 = input.data._uint32;
      out->unionCase = 1;
      break;
    case EndDeviceProtocol_Data__uint64_tag:
      out->data._uint32 = input.data._uint64;
      out->unionCase = 1;
      break;
    case EndDeviceProtocol_Data__int8_tag:
      out->data._int = input.data._int8;
      out->unionCase = 2;
      break;
    case EndDeviceProtocol_Data__int16_tag:
      out->data._int = input.data._int16;
      out->unionCase = 2;
      break;
    case EndDeviceProtocol_Data__int32_tag:
      out->data._int = input.data._int32;
      out->unionCase = 2;
      break;
    case EndDeviceProtocol_Data__int64_tag:
      out->data._int = input.data._int64;
      out->unionCase = 2;
      break;
    case EndDeviceProtocol_Data__float_tag:
      out->data._float = input.data._float;
      out->unionCase = 3;
      break;
    case EndDeviceProtocol_Data__double_tag:
      out->data._double = input.data._double;
      out->unionCase = 4;
      break;
    default:
      // Unknown data type
      printf("Unknown data type!\n");
      break;
  }
}

void free_message(Message *msg){
  for(int i = 0; i < msg->amount; i++){
    //queries
    for(int q = 0; q < msg->queries[i].amount; q++){
      //operations
      if(msg->queries[i].operations[q].unionCase == 0){
        //map
        for(int m = 0; m < msg->queries[i].operations[q].operation.map->expression->p_size; m++){
          free(&msg->queries[i].operations[q].operation.map->expression->program[m]);
        }
        free(msg->queries[i].operations[q].operation.map->expression);
        free(msg->queries[i].operations[q].operation.map);
      } else if (msg->queries[i].operations[q].unionCase == 1){
        //filter
        for(int m = 0; m < msg->queries[i].operations[q].operation.filter->predicate->p_size; m++){
          free(&msg->queries[i].operations[q].operation.filter->predicate->program[m]);
        }
        free(msg->queries[i].operations[q].operation.filter->predicate);
        free(msg->queries[i].operations[q].operation.filter);
      } else if (msg->queries[i].operations[q].unionCase == 2){
        //window
        free(msg->queries[i].operations[q].operation.window);
      } else {
        printf("Unkown operation type!\n");
      }
      free(&msg->queries[i].operations[q]);
    }
    free(msg->queries[i].operations);
  }
  free(msg->queries);
  free(msg);
}
