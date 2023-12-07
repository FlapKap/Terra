#ifndef LORA_H
#define LORA_H

#include <stdlib.h>
#include <stdint.h>

bool lorawan_send_message(uint8_t* serializedData, uint8_t len);
bool lorawan_receive_message(void);
int lorawan_initialize_lorawan(void);
int lorawan_connect_lorawan(void);
void lorawan_print_connection_info(void);
#endif /* LORA_H */
