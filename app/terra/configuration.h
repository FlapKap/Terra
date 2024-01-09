#ifndef CONFIGURATION_H
#define CONFIGURATION_H



#define CONFIGURATION_EEPROM_START (SEMTECH_LORAMAC_EEPROM_START + SEMTECH_LORAMAC_EEPROM_LEN)
#define CONFIGURATION_MAGIC "Terra"
#define CONFIGURATION_MAGIC_SIZE sizeof(CONFIGURATION_MAGIC)
#define CONFIGURATION_LOOP_COUNTER_TYPE uint32_t
#define CONFIGURATION_LOOP_COUNTER_SIZE sizeof(CONFIGURATION_LOOP_COUNTER_TYPE)
#define CONFIGURATION_QUERY_SIZE TerraProtocol_Message_size

#define CONFIGURATION_TERRA_CONFIGURATION_SIZE (\
    CONFIGURATION_LOOP_COUNTER_SIZE +\
    CONFIGURATION_QUERY_SIZE)

#ifndef DISABLE_LORA 
#include "semtech_loramac.h"
#endif
#include <terraprotocol.pb.h>


typedef struct _TerraConfiguration {
    CONFIGURATION_LOOP_COUNTER_TYPE loop_counter;
    TerraProtocol_Message* message;
#ifndef DISABLE_LORA
    semtech_loramac_t* loramac;
#endif
} TerraConfiguration;

bool configuration_save( TerraConfiguration* config );
bool configuration_load( TerraConfiguration* config );
#endif // CONFIGURATION_H