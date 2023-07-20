#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "EndDeviceProtocol.pb.h"
#include "operators.h"
#include "pb_encode.h"

void init_data(Instruction *instr, EndDeviceProtocol_Data *out);
void init_query_response(QueryResponse *query, EndDeviceProtocol_Output_QueryResponse *out);
void init_output(OutputMessage *_output, EndDeviceProtocol_Output *out);

bool encode_output_message(pb_ostream_t * stream, OutputMessage * msg);
bool decode_output_message(pb_istream_t *stream, EndDeviceProtocol_Output *output);

#endif