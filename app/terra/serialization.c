#include "serialization.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb_common.h"
#include <assert.h>
#include "print_utils.h"

#define ENABLE_DEBUG 0
#include "debug.h"

bool serialization_deserialize_message(const uint8_t *data, const size_t len, TerraProtocol_Message *dest_msg)
{
assert(len > 0);
  pb_istream_t istream = pb_istream_from_buffer(data, len);
  bool res = pb_decode(&istream, TerraProtocol_Message_fields, dest_msg);
  if (res)
  {
    DEBUG("[serialization.c] deserialized message of length %d\n", len);
    print_terraprotocol_message(dest_msg);
  }
  return res;
}


bool serialization_serialize_message(const TerraProtocol_Message *msg, uint8_t *dest_data, const size_t dest_len, size_t *bytes_written)
{
  pb_ostream_t ostream = pb_ostream_from_buffer(dest_data, dest_len);
  bool res = pb_encode(&ostream, TerraProtocol_Message_fields, msg);
  if (res)
  {
    DEBUG("[serialization.c] serialized message of length %d\n", ostream.bytes_written);
  }
  assert(ostream.bytes_written <= dest_len);
  *bytes_written = ostream.bytes_written;
  return res;
}

bool serialization_serialize_output(const TerraProtocol_Output *out, uint8_t *dest_data, const size_t dest_len, size_t *bytes_written)
{
  pb_ostream_t ostream = pb_ostream_from_buffer(dest_data, dest_len);
  bool res = pb_encode(&ostream, TerraProtocol_Output_fields, out);
  if (res)
  {
    DEBUG("[serialization.c] serialized output of length %d\n", ostream.bytes_written);
  }
  assert(ostream.bytes_written <= dest_len);
  *bytes_written = ostream.bytes_written;
  return res;
}