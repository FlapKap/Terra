#ifndef LORA_H
#define LORA_H

#include <stdlib.h>
#include <stdint.h>

bool lorawan_send_message(uint8_t* serializedData, uint8_t len);

int lorawan_initialize_lorawan(void);
int lorawan_connect_lorawan(void);

#endif /* LORA_H */