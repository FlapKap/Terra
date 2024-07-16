#ifndef CONFIGURATION_H
#define CONFIGURATION_H



#define CONFIGURATION_EEPROM_START (SEMTECH_LORAMAC_EEPROM_START + SEMTECH_LORAMAC_EEPROM_LEN)
#define CONFIGURATION_MAGIC "Terra"
#define CONFIGURATION_MAGIC_SIZE sizeof(CONFIGURATION_MAGIC)
#define CONFIGURATION_LOOP_COUNTER_TYPE uint32_t
#define CONFIGURATION_LOOP_COUNTER_SIZE sizeof(CONFIGURATION_LOOP_COUNTER_TYPE)
#define CONFIGURATION_QUERY_MAX_SIZE TerraProtocol_Message_size

#define CONFIGURATION_LORAWAN_CONFIG_SIZE  (\
        LORAMAC_DEVEUI_LEN +               \
        LORAMAC_APPEUI_LEN +               \
        LORAMAC_APPKEY_LEN +               \
        LORAMAC_APPSKEY_LEN +              \
        LORAMAC_NWKSKEY_LEN +              \
        LORAMAC_DEVADDR_LEN +              \
        LORAMAC_UPLINK_COUNTER_LEN +       \
        LORAMAC_RX2_FREQ_LEN +             \
        LORAMAC_RX2_DR_LEN +               \
        sizeof(bool)) // join_state

#define CONFIGURATION_TERRA_CONFIGURATION_SIZE (\
    CONFIGURATION_LOOP_COUNTER_SIZE +\
    CONFIGURATION_QUERY_MAX_SIZE +\
    sizeof(uint32_t)) // raw_message_size

#define CONFIGURATION_SIZE (\
    CONFIGURATION_MAGIC_SIZE +\
    CONFIGURATION_TERRA_CONFIGURATION_SIZE +\
    CONFIGURATION_LORAWAN_CONFIG_SIZE)

#ifndef DISABLE_LORA 
#include "semtech_loramac.h"
#endif

#ifdef DISABLE_LORA
#define LORAWAN_APP_DATA_MAX_SIZE 242 // Since we dont have the semtech_loramac.h where it normally is
#endif

#include <terraprotocol.pb.h>
#include <stdint.h>
#include "number.h"
#ifndef MAX_WINDOW_OPERATORS
#define MAX_WINDOW_OPERATORS 5
#endif

typedef enum _windowState {
    WINDOW_STATE_NOT_INITIALIZED,
    WINDOW_STATE_RUNNING,
    WINDOW_STATE_FINISHED
} windowState;

/**
 * @brief This struct contains all the data a window needs to keep accross executions
 * These values are saved in the configuration
 */
typedef struct _WindowData {
    Number aggregation_value;
    Number start_value;
    Number end_value;

    uint32_t count;
    // field to check if this window is ready to be used or just a default value
    windowState state;
} WindowData;

typedef struct _TerraConfiguration {
    CONFIGURATION_LOOP_COUNTER_TYPE loop_counter;
    uint8_t raw_message_size;
    uint8_t raw_message_buffer[LORAWAN_APP_DATA_MAX_SIZE];
    WindowData window_data[MAX_WINDOW_OPERATORS]; // we match window_data by order of window operations. For example window_data[0] is matched to the first window in the deserialized and executed message
} TerraConfiguration;

/**
 * @brief
 * @param config
 * @param loramac_config
 * @param save_query set to true if the raw message changed in the buffer. Otherwise it wont save that
 * @return true if the save was successful. False otherwise
 * @note this might save to flash. This causes wear so use it as little as possible
 */
bool configuration_save( TerraConfiguration* config, semtech_loramac_t* loramac_config, bool save_query);
bool configuration_load( TerraConfiguration* config, semtech_loramac_t* loramac_config );
#endif // CONFIGURATION_H
