#include "operators.h"
#include <stdbool.h>
#ifndef DISABLE_LORA
#include "semtech_loramac.h"
extern semtech_loramac_t loramac;
#endif
bool network_initialize_network(void);

bool network_is_connected(void);

//bool network_has_valid_message(void);

bool network_get_message(uint8_t* out, const size_t out_len, uint8_t* bytes_written);
bool network_send_message(uint8_t* msg, const size_t msg_len);
bool network_send_heartbeat(void);

