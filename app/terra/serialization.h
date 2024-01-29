#include "terraprotocol.pb.h"
#include <inttypes.h>

bool serialization_deserialize_message(const uint8_t* data, const size_t len, TerraProtocol_Message* dest_msg);
bool serialization_serialize_message(const TerraProtocol_Message* msg, uint8_t* dest_data, const size_t dest_len, size_t* bytes_written);
bool serialization_serialize_output(const TerraProtocol_Output* out, uint8_t* dest_data, const size_t dest_len, size_t* bytes_written);