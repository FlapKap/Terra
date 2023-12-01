#include "configuration.h"
#include "periph/eeprom.h"

#define ENABLE_DEBUG 0
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

void configuration_save(TerraConfiguration *config)
{
    uint32_t pos = CONFIGURATION_EEPROM_START;
    pos += eeprom_write(pos, CONFIGURATION_MAGIC, CONFIGURATION_MAGIC_SIZE);
    pos += _write_uint32(pos, config->loop_counter);
    pos += eeprom_write(pos, config->query, CONFIGURATION_QUERY_SIZE);
    semtech_loramac_save_config(config->loramac);
}

bool configuration_load( TerraConfiguration* config )
{
    uint32_t pos = CONFIGURATION_EEPROM_START;
    uint8_t magic[CONFIGURATION_MAGIC_SIZE] = { 0 };
    pos += eeprom_read(pos, magic, CONFIGURATION_MAGIC_SIZE);
    if (memcmp(magic, CONFIGURATION_MAGIC, CONFIGURATION_MAGIC_SIZE) != 0) {
        return false;
    }
    pos += _read_uint32(pos, &config->loop_counter);
    pos += eeprom_read(pos, config->query, CONFIGURATION_QUERY_SIZE);
    //loramac automatically loads
    return true;
}