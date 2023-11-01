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
#include "log.h"
#ifndef DISABLE_LORA
#include "lorawan.h"
#include "semtech_loramac.h"
extern semtech_loramac_t loramac;



#endif
//TODO: Message need to be properly initialized
static Message message = { 0 };

// we provide two implementations: one for lora, and one with a default message defined
#if DISABLE_LORA
static const pb_byte_t raw_message[] = DEFAULT_QUERY_AS_PB_CHAR_ARRAY;




bool network_initialize_network(void){
  LOG_DEBUG("network_initialize_network: DISABLE_LORA enabled so no network is actually initialized\n");
  pb_istream_t stream = pb_istream_from_buffer(raw_message, sizeof(raw_message));
  bool res = decode_input_message(&stream, &message);
  print_message(&message);
  return res;
}

bool network_is_connected(void){
  LOG_DEBUG("network_is_connected: DISABLE_LORA enabled so no network is actually connected\n");
  return true;
}

bool network_has_valid_message(void){
  LOG_DEBUG("network_has_valid_message: DISABLE_LORA enabled so using predefined message\n");
  return true;
}

Message network_get_message(void){
  return message;
}
bool network_send_message(OutputMessage msg){
  LOG_DEBUG("network_send_message: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
  print_output_message(&msg);
  return true;
}

bool network_send_heartbeat(void){
  LOG_DEBUG("network_send_heartbeat: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
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
  LOG_INFO("Initializing network\n");
  int ret_init = lorawan_initialize_lorawan();
  lorawan_print_connection_info();
  int ret_connect = lorawan_connect_lorawan();

  // check if init and connect were successful
  if (ret_init == 0 && ret_connect == 0){
    return true;
  } else {
    LOG_ERROR("Failed to initialize network\n");
    return false;
  }
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
    LOG_INFO("Sending message with length %d\n", ostream.bytes_written);
  return lorawan_send_message(buffer, (uint8_t)256);
}

bool network_send_heartbeat(void){
  return lorawan_send_message((uint8_t[]){'<','3'}, 2);
}

#endif
