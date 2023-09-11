#include <fmt.h>
#include <base64.h>
#include "network.h"

#include "operators.h"

#include "EndDeviceProtocol.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "input_serialization.h"
#include "output_serialization.h"
#include "print_utils.h"
#ifndef DISABLE_LORA
#include "lorawan.h"
#include "semtech_loramac.h"
extern semtech_loramac_t loramac;


#endif

// Debug
#define ENABLE_DEBUG 1
#include "debug.h"

static Message message;

// we provide two implementations: one for lora, and one with a default message defined
#if DISABLE_LORA
static const pb_byte_t raw_message[] = DEFAULT_QUERY_AS_PB_CHAR_ARRAY;




bool network_initialize_network(void){
  DEBUG("network_initialize_network: DISABLE_LORA enabled so no network is actually initialized\n");
  pb_istream_t stream = pb_istream_from_buffer(raw_message, sizeof(raw_message));
  bool res = decode_input_message(&stream, &message);
  print_message(&message);
  return res;
}

bool network_is_connected(void){
  DEBUG("network_is_connected: DISABLE_LORA enabled so no network is actually connected\n");
  return true;
}

bool network_has_valid_message(void){
  DEBUG("network_has_valid_message: DISABLE_LORA enabled so using predefined message\n");
  return true;
}

Message network_get_message(void){
  return message;
}
bool network_send_message(OutputMessage msg){
  DEBUG("network_send_message: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
  print_output_message(&msg);
  return true;
}

bool network_send_heartbeat(void){
  DEBUG("network_send_heartbeat: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
  return true;
}

#else
static bool receive_and_decode(void)
{
  // Check for received messages
  Message tmp;
  pb_istream_t istream = pb_istream_from_buffer(loramac.rx_data.payload, loramac.rx_data.payload_len);
  bool res = decode_input_message(&istream, &tmp);
  if (res)
  {
    message = tmp;
  }
    return res;
}


bool network_initialize_network(void){
  DEBUG("Initializing network");
  int ret_init = lorawan_initialize_lorawan();
  int ret_connect = lorawan_connect_lorawan();
#if ENABLE_DEBUG
  // fputs to avoid newlines in the end
  fputs("  Device EUI: ", stdout);
  print_bytes_hex(loramac.deveui, LORAMAC_DEVEUI_LEN);
  fputs("\n  Application EUI: ", stdout);
  print_bytes_hex(loramac.appeui, LORAMAC_APPEUI_LEN);
  fputs("\n  Application Key: ", stdout);
  print_bytes_hex(loramac.appkey, LORAMAC_APPKEY_LEN);
  puts("\n");
#endif
  
  return ret_init == 0 && ret_connect == 0;
}

bool network_has_valid_message(void){
  //TODO: right now we define valid message as a message with amount > 0, but amount == 0 could be used to cancel message. This should be revisited
  return message.amount > 0;
}

Message network_get_message(void){
  receive_and_decode(); //TODO: utilize the _recv thread msg queue to only update message when new message has actually been received
  return message;
}

bool network_send_message(OutputMessage msg){
  //encode
  uint8_t buffer[256];
  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  encode_output_message(&ostream, &msg);

  //send
  return lorawan_send_message(buffer, (uint8_t)256);
}

bool network_send_heartbeat(void){
  lorawan_send_message((uint8_t[]){'<','3'}, 2);
  return true;
}

#endif