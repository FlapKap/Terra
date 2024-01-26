#ifndef CONFIGURATION_H
#define CONFIGURATION_H



#define CONFIGURATION_EEPROM_START (SEMTECH_LORAMAC_EEPROM_START + SEMTECH_LORAMAC_EEPROM_LEN)
#define CONFIGURATION_MAGIC "Terra"
#define CONFIGURATION_MAGIC_SIZE sizeof(CONFIGURATION_MAGIC)
#define CONFIGURATION_LOOP_COUNTER_TYPE uint32_t
#define CONFIGURATION_LOOP_COUNTER_SIZE sizeof(CONFIGURATION_LOOP_COUNTER_TYPE)
#define CONFIGURATION_QUERY_MAX_SIZE TerraProtocol_Message_size

#define CONFIGURATION_TERRA_CONFIGURATION_SIZE (\
    CONFIGURATION_LOOP_COUNTER_SIZE +\
    CONFIGURATION_QUERY_MAX_SIZE)

#ifndef DISABLE_LORA 
#include "semtech_loramac.h"
#endif

#ifdef DISABLE_LORA
#define LORAWAN_APP_DATA_MAX_SIZE 242 // Since we dont have the semtech_loramac.h where it normally is
#endif

#include <terraprotocol.pb.h>


typedef struct _TerraConfiguration {
    CONFIGURATION_LOOP_COUNTER_TYPE loop_counter;
    uint8_t raw_message[LORAWAN_APP_DATA_MAX_SIZE];
    uint8_t message_size;
#ifndef DISABLE_LORA
    semtech_loramac_t* loramac;
#endif
} TerraConfiguration;

bool configuration_save( TerraConfiguration* config );
bool configuration_load( TerraConfiguration* config );
#endif // CONFIGURATION_H
