#include "configuration.h"
#include "eepreg.h"
#include "periph/eeprom.h"


#ifndef DISABLE_LORA
#include "lorawan.h"
#endif

#define ENABLE_DEBUG 1
#include "debug.h"

// useful functions stolen from semtech_loramac.c
static size_t _read_uint32(size_t pos, uint32_t *value)
{
    uint8_t array[4] = { 0 };
    size_t ret = eeprom_read(pos, array, sizeof(uint32_t));
    *value = ((uint32_t)array[0] << 24 | (uint32_t)array[1] << 16 |
              (uint32_t)array[2] << 8 | array[3]);
    return ret;
}

static size_t _write_uint32(size_t pos, uint32_t value)
{
    uint8_t array[4] = { 0 };
    array[0] = (uint8_t)(value >> 24);
    array[1] = (uint8_t)(value >> 16);
    array[2] = (uint8_t)(value >> 8);
    array[3] = (uint8_t)(value);
    return eeprom_write(pos, array, sizeof(uint32_t));
}

bool configuration_save(TerraConfiguration *config)
{   
    DEBUG("[configuration.c] Saving configuration\n");

    uint32_t pos;
    int res = eepreg_add(&pos, CONFIGURATION_MAGIC, CONFIGURATION_TERRA_CONFIGURATION_SIZE);
    switch (res)
    {
    case -EIO:
        DEBUG("[configuration.c] eepreg: I/O error when saving configuration\n");
        return false;
        break;
    case -ENOSPC:
        DEBUG("[configuration.c] eepreg: No space error when saving configuration\n");
        return false;
        break;
    case -EADDRINUSE:
        DEBUG("[configuration.c] eepreg: Address already in use when saving configuration\n");
        return false;
    case 0: // success
        pos += eeprom_write(pos, &config->raw_message_size, sizeof(config->raw_message_size));
        pos += eeprom_write(pos, &config->raw_message_buffer, config->raw_message_size);
        pos += _write_uint32(pos, config->loop_counter);
#if !(defined(APPLICATION_RUN_TEST) || defined(DISABLE_LORA))
        DEBUG("Saving loramac\n");
        semtech_loramac_save_config(config->loramac);
#endif  
        return true;
        break;
    default:
        DEBUG("[configuration.c] eepreg: Unknown error when saving configuration\n");
        return false;
    }
    return true;
}

bool configuration_load( TerraConfiguration* config )
{
    DEBUG("[configuration.c] Loading configuration\n");
    uint32_t pos;

    int res = eepreg_read(&pos, CONFIGURATION_MAGIC);
    switch (res)
    {
    case -ENOENT:
        DEBUG("[configuration.c] eepreg: No configuration found\n");
        return false;
        break;
    case -EIO:
        DEBUG("[configuration.c] eepreg: I/O error when loading configuration\n");
        return false;
        break;
    case 0: // success
    {
        pos += eeprom_read(pos, &(config->raw_message_size), sizeof(config->raw_message_size));
        pos += eeprom_read(pos, &config->raw_message_buffer, config->raw_message_size);
        pos += _read_uint32(pos, &config->loop_counter);
        break;
    }

    default:
        DEBUG("[configuration.c] eepreg: Unknown error when loading configuration\n");
        return false;
    }

    //loramac automatically loads


    
    return true;
}
