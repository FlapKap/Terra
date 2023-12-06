#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "periph/eeprom.h"
#ifndef DISABLE_LORA
#include "semtech_loramac.h"
#endif
#include <terraprotocol.pb.h>

#define CONFIGURATION_EEPROM_START 0
#define CONFIGURATION_MAGIC ((char[]){'T', 'e', 'r', 'r', 'a'})
#define CONFIGURATION_MAGIC_SIZE sizeof(CONFIGURATION_MAGIC)
#define CONFIGURATION_LOOP_COUNTER_TYPE uint32_t
#define CONFIGURATION_LOOP_COUNTER_SIZE sizeof(CONFIGURATION_LOOP_COUNTER_TYPE)
#define CONFIGURATION_QUERY_SIZE TerraProtocol_Message_size

#define CONFIGURATION_TERRA_CONFIGURATION_SIZE \
    CONFIGURATION_MAGIC_SIZE +\
    CONFIGURATION_LOOP_COUNTER_SIZE +\
    CONFIGURATION_QUERY_SIZE

// Redefine the EEPROM start address for loramac configuration to be after our own
// Since loramac doesn't define an end position or size pre-processor time it's easier
// to just save it at the end
// TODO: modify loramac code to provide end size. only then can we verify that there is space in eeprom
#undef SEMTECH_LORAMAC_EEPROM_START
#define SEMTECH_LORAMAC_EEPROM_START CONFIGURATION_EEPROM_START + CONFIGURATION_TERRA_CONFIGURATION_SIZE

typedef struct _TerraConfiguration {
    CONFIGURATION_LOOP_COUNTER_TYPE loop_counter;
    TerraProtocol_Message* query;
#ifndef DISABLE_LORA
    semtech_loramac_t* loramac;
#endif
} TerraConfiguration;

void configuration_save( TerraConfiguration* config );
bool configuration_load( TerraConfiguration* config );
#endif // CONFIGURATION_H