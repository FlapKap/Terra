#include <fmt.h>
#include <base64.h>
#include "network.h"

#include "operators.h"
#include "environment.h"
#include "terraprotocol.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "print_utils.h"
#include "log.h"
#ifndef DISABLE_LORA
#include "lorawan.h"
extern semtech_loramac_t loramac;
#endif

// we provide two implementations: one for lora, and one with a default message defined
#if DISABLE_LORA
static const pb_byte_t raw_message[] = DEFAULT_QUERY_AS_PB_CHAR_ARRAY;


bool network_initialize_network(void){
  LOG_DEBUG("network_initialize_network: DISABLE_LORA enabled so no network is actually initialized\n");

  return true;
}

bool network_is_connected(void){
  LOG_DEBUG("network_is_connected: DISABLE_LORA enabled so no network is actually connected\n");
  return true;
}

bool network_has_valid_message(void){
  LOG_DEBUG("network_has_valid_message: DISABLE_LORA enabled so using predefined message\n");
  return true;
}

bool network_get_message(uint8_t* out, const size_t out_len, uint8_t* bytes_written){
  LOG_DEBUG("network_get_message: DISABLE_LORA enabled so using predefined message\n");
  assert(bytes_written != NULL);
  assert(out != NULL);
  assert(out_len >= sizeof(raw_message));
  *bytes_written = sizeof(raw_message);

  memcpy(out, raw_message, sizeof(raw_message));

  return true;
}
bool network_send_message(TerraProtocol_Output* msg){
  LOG_DEBUG("network_send_message: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
  print_terraprotocol_output_message(msg);
  return true;
}

bool network_send_heartbeat(void){
  LOG_DEBUG("network_send_heartbeat: DISABLE_LORA enabled so no message is actually sent. Just logged.\n");
  return true;
}

#else


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

bool network_get_message(uint8_t* out, const size_t out_len, uint8_t* bytes_written){
  // TODO: implement this
  assert(out != NULL);
  assert(bytes_written != NULL);
  assert(loramac.rx_data.payload_len > 0);
  assert(loramac.rx_data.payload_len <= out_len);
  // Check for received messages

  memcpy(out, loramac.rx_data.payload, loramac.rx_data.payload_len);
  *bytes_written = loramac.rx_data.payload_len;

  LOG_DEBUG("[network.c] Got message of length %d\n", *bytes_written);
  
  
  return true;
}

bool network_send_message(uint8_t* msg, const size_t msg_len){
  assert(msg_len <= LORAWAN_APP_DATA_MAX_SIZE);
  assert(msg != NULL);
  return lorawan_send_message(msg, msg_len);
  
  // uint8_t buffer[255];
  // pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  // bool res = pb_encode(&ostream, TerraProtocol_Output_fields, msg);
  // if (!res){
  //   LOG_ERROR("Failed to encode message\n");
  //   return false;
  // }

  // //send
  // LOG_INFO("Sending message with length %d\n", ostream.bytes_written);
  // return lorawan_send_message(buffer, sizeof(buffer));
}

bool network_send_heartbeat(void){
  static uint8_t heart[] = {'<','3'}; 
  return lorawan_send_message(heart, sizeof(heart));
}

#endif
