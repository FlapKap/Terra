#ifndef ENCODEINPUT_H
#define ENCODEINPUT_H


#include <stdlib.h>
#include <stdio.h>
#include "EndDeviceProtocol.pb.h"
#include "operators.h"

// An instruction corresponds to the Data type in .proto
bool encode_message(pb_ostream_t *stream, Message message);
bool decode_input_message(pb_istream_t *stream, Message *out);

void decoded_input_to_message(EndDeviceProtocol_Message decoded, Message *out);
void decoded_input_to_query(EndDeviceProtocol_Query decoded, Query *out);
void decoded_input_to_operation(EndDeviceProtocol_Operation decoded, Operation *out);
void decoded_input_to_window_operation(EndDeviceProtocol_WindowOperation decoded, Window *out);
void decoded_input_to_filter_operation(EndDeviceProtocol_FilterOperation decoded, Filter *out);
void decoded_input_to_map_operation(EndDeviceProtocol_MapOperation decoded, Map *out);
void decoded_input_to_instruction(EndDeviceProtocol_Data decoded, Instruction *out);

void init_message(Message msg, EndDeviceProtocol_Message *out);
void init_query(Query query, EndDeviceProtocol_Query *out);
void init_operation(Operation op, EndDeviceProtocol_Operation *out);
void init_window_operation(Window window, EndDeviceProtocol_WindowOperation *out);
void init_filter_operation(Filter filter, EndDeviceProtocol_FilterOperation *out);
void init_map_operation(Map map, EndDeviceProtocol_MapOperation *out);

void free_message(Message *msg);

#endif
