#include "configuration.h"

#ifndef DISABLE_LORA
#include "lorawan.h"
#include "semtech_loramac.h"
#endif

#define ENABLE_DEBUG 0
#include "debug.h"

#include "assert.h"


TerraConfiguration config = {
    .raw_message_size = sizeof((uint8_t[])DEFAULT_QUERY_AS_PB_CHAR_ARRAY),
    .raw_message_buffer = DEFAULT_QUERY_AS_PB_CHAR_ARRAY,
    .loop_counter = 0
};


#ifdef CPU_ESP32
static TerraConfiguration config_stored __attribute__((section(".rtc.bss")));
// loramac config values
static uint8_t  loramac_deveui[LORAMAC_DEVEUI_LEN] __attribute__((section(".rtc.bss")));
static uint8_t loramac_appeui[LORAMAC_APPEUI_LEN] __attribute__((section(".rtc.bss")));
static uint8_t loramac_appkey[LORAMAC_APPKEY_LEN] __attribute__((section(".rtc.bss")));
static uint8_t loramac_appskey[LORAMAC_APPSKEY_LEN] __attribute__((section(".rtc.bss")));
static uint8_t loramac_nwkskey[LORAMAC_NWKSKEY_LEN] __attribute__((section(".rtc.bss")));
static uint8_t loramac_devaddr[LORAMAC_DEVADDR_LEN] __attribute__((section(".rtc.bss")));
static uint32_t loramac_uplink_counter __attribute__((section(".rtc.bss")));
static uint32_t loramac_rx2_freq __attribute__((section(".rtc.bss")));
static uint8_t loramac_rx2_dr __attribute__((section(".rtc.bss")));
static bool loramac_join_state __attribute__((section(".rtc.bss")));


bool configuration_load(TerraConfiguration *config, semtech_loramac_t *loramac)
{
    DEBUG("[configuration.c] Loading configuration\n");
    memcpy(config, &config_stored, sizeof(TerraConfiguration));

    // loramac config values
    semtech_loramac_set_deveui(loramac, loramac_deveui);
    semtech_loramac_set_appeui(loramac, loramac_appeui);
    semtech_loramac_set_appkey(loramac, loramac_appkey);
    semtech_loramac_set_appskey(loramac, loramac_appskey);
    semtech_loramac_set_nwkskey(loramac, loramac_nwkskey);
    semtech_loramac_set_devaddr(loramac, loramac_devaddr);
    semtech_loramac_set_uplink_counter(loramac, loramac_uplink_counter);
    semtech_loramac_set_rx2_freq(loramac, loramac_rx2_freq);
    semtech_loramac_set_rx2_dr(loramac, loramac_rx2_dr);
    semtech_loramac_set_join_state(loramac, loramac_join_state);
    return true;
}

bool configuration_save(TerraConfiguration *config, semtech_loramac_t *loramac, bool save_query)
{
    DEBUG("[configuration.c] Saving configuration\n");
    if (save_query)
    {
        memcpy(&config_stored, config, sizeof(TerraConfiguration));
    }
    // loramac config values
    semtech_loramac_get_deveui(loramac, loramac_deveui);
    semtech_loramac_get_appeui(loramac, loramac_appeui);
    semtech_loramac_get_appkey(loramac, loramac_appkey);
    semtech_loramac_get_appskey(loramac, loramac_appskey);
    semtech_loramac_get_nwkskey(loramac, loramac_nwkskey);
    semtech_loramac_get_devaddr(loramac, loramac_devaddr);
    loramac_uplink_counter =semtech_loramac_get_uplink_counter(loramac);
    loramac_rx2_freq = semtech_loramac_get_rx2_freq(loramac);
    loramac_rx2_dr = semtech_loramac_get_rx2_dr(loramac);
    loramac_join_state = semtech_loramac_get_join_state(loramac);
    return true;
}

#elif defined(MODULE_EEPREG)
#include "eepreg.h"
#include "periph/eeprom.h"
// useful functions stolen from semtech_loramac.c
static size_t _read_uint32(size_t pos, uint32_t *value)
{
    uint8_t array[4] = {0};
    size_t ret = eeprom_read(pos, array, sizeof(uint32_t));
    *value = ((uint32_t)array[0] << 24 | (uint32_t)array[1] << 16 |
              (uint32_t)array[2] << 8 | array[3]);
    return ret;
}

static size_t _write_uint32(size_t pos, uint32_t value)
{
    uint8_t array[4] = {0};
    array[0] = (uint8_t)(value >> 24);
    array[1] = (uint8_t)(value >> 16);
    array[2] = (uint8_t)(value >> 8);
    array[3] = (uint8_t)(value);
    return eeprom_write(pos, array, sizeof(uint32_t));
}

bool configuration_save(TerraConfiguration *config, semtech_loramac_t *loramac, bool save_query)
{
    DEBUG("[configuration.c] Saving configuration\n");

    uint32_t pos;
    int res = eepreg_add(&pos, CONFIGURATION_MAGIC, sizeof(TerraConfiguration));
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
    if (save_query) {
        pos += eeprom_write(pos, &config->raw_message_size, sizeof(config->raw_message_size));
        pos += eeprom_write(pos, &config->raw_message_buffer, config->raw_message_size);
    } else {
            pos += sizeof(config->raw_message_size) + config->raw_message_size;
        }
        pos += _write_uint32(pos, config->loop_counter);
        
        pos += eeprom_write(pos, &config->window_data, sizeof(config->window_data));
        #if !(defined(APPLICATION_RUN_TEST) || defined(DISABLE_LORA))
        DEBUG("Saving loramac\n");
        semtech_loramac_save_config(loramac);
#endif
        return true;
        break;
    default:
        DEBUG("[configuration.c] eepreg: Unknown error when saving configuration\n");
        return false;
    }
    return true;
}

bool configuration_load(TerraConfiguration *config, __attribute__((unused)) semtech_loramac_t *loramac)
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

        pos += eeprom_read(pos, &config->window_data, sizeof(config->window_data));
        break;
    }

    default:
        DEBUG("[configuration.c] eepreg: Unknown error when loading configuration\n");
        return false;
    }
    // loramac automatically loads if eepreg is there
    return true;
}

#elif CONFIG_SLOT_AUX_LEN

#include "mtd.h"
#include "mtd_flashpage.h"

#ifndef CONFIGURATION_MTD_SECTOR
#define CONFIGURATION_MTD_SECTOR 0
#endif
#if CONFIGURATION_MTD_SECTOR < 0
#error "CONFIGURATION_MTD_SECTOR must be >= 0"
#endif

#ifndef CONFIGURATION_MTD_DEVICE
#define CONFIGURATION_MTD_DEVICE mtd_aux
#endif

uint8_t config_buffer[CONFIGURATION_SIZE] __attribute__((aligned(FLASHPAGE_WRITE_BLOCK_ALIGNMENT)));

static inline uint32_t _get_number_of_sectors_for_buffer(void)
{
    uint32_t num_sectors = sizeof(config_buffer) / (CONFIGURATION_MTD_DEVICE->pages_per_sector * CONFIGURATION_MTD_DEVICE->page_size);
    assert(num_sectors > 0 && num_sectors <= CONFIGURATION_MTD_DEVICE->sector_count);
    DEBUG("[configuration.c] num sectors: %" PRIdSIZE " / ( %" PRIu32 " * %" PRIu32 ") %" PRIu32 "\n", sizeof(config_buffer), CONFIGURATION_MTD_DEVICE->pages_per_sector, CONFIGURATION_MTD_DEVICE->page_size, num_sectors);
    return num_sectors;
}

bool configuration_save(TerraConfiguration *config, semtech_loramac_t *loramac_config, bool save_query)
{
    static_assert(sizeof(*config) == sizeof(TerraConfiguration), "config doesnt have type of TerraConfiguration"); // just sanitycheck that if we change the type of config we also need to change the type of config_buffer
    // take both app config and loramac config and write to mtd flash. Need to take care of flash page boundaries. Possibly also the erase state of bytes

    // assert we can fit the buffer within a single sector
    assert(sizeof(config_buffer) < CONFIGURATION_MTD_DEVICE->sector_count * CONFIGURATION_MTD_DEVICE->pages_per_sector * CONFIGURATION_MTD_DEVICE->page_size);
    assert(CONFIGURATION_MTD_DEVICE->sector_count > CONFIGURATION_MTD_SECTOR);

    uint8_t* config_bufferPtr = config_buffer;
    // copy magic word into buffer
    if (strcpy((char *) config_bufferPtr, CONFIGURATION_MAGIC) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy magic word to buffer\n");
        return false;
    };
    config_bufferPtr += CONFIGURATION_MAGIC_SIZE;

    // copy terraconfig into the buffer
    semtech_loramac_get_deveui(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_DEVEUI_LEN;

    semtech_loramac_get_appeui(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPEUI_LEN;

    semtech_loramac_get_appkey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPKEY_LEN;

    semtech_loramac_get_nwkskey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_NWKSKEY_LEN;

    semtech_loramac_get_appskey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPSKEY_LEN;

    semtech_loramac_get_devaddr(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_DEVADDR_LEN;

    uint32_t rx2_freq = semtech_loramac_get_rx2_freq(loramac_config);
    *config_bufferPtr++ = (rx2_freq >> 24) & 0xFF;
    *config_bufferPtr++ = (rx2_freq >> 16) & 0xFF;
    *config_bufferPtr++ = (rx2_freq >> 8) & 0xFF;
    *config_bufferPtr++ = rx2_freq & 0xFF;

    uint8_t rx2_dr = semtech_loramac_get_rx2_dr(loramac_config);
    *config_bufferPtr++ = rx2_dr;

    bool joined = semtech_loramac_get_join_state(loramac_config);
    *config_bufferPtr++ = joined;

    // copy terraconfig into the buffer
    uint32_t loop_counter = config->loop_counter;
    *config_bufferPtr++ = (loop_counter >> 24) & 0xFF;
    *config_bufferPtr++ = (loop_counter >> 16) & 0xFF;
    *config_bufferPtr++ = (loop_counter >> 8) & 0xFF;
    *config_bufferPtr++ = loop_counter & 0xFF;

    *config_bufferPtr++ = config->raw_message_size;
    if (save_query) {
        memcpy(config_bufferPtr, config->raw_message_buffer, config->raw_message_size);
    }
    config_bufferPtr += config->raw_message_size;

    memcpy(config_bufferPtr, config->window_data, sizeof(config->window_data));
    config_bufferPtr += sizeof(config->window_data);

    // write the buffer and handle any return errors TODO: make this raw_message_changed_aware
    int res = mtd_write_sector(CONFIGURATION_MTD_DEVICE, config_buffer, CONFIGURATION_MTD_SECTOR, _get_number_of_sectors_for_buffer());
    switch (res)
    {
    case 0:
        DEBUG("[configuration.c] mtd: Configuration saved successfully\n");
        return true;
        break;
    case -ENODEV:
        DEBUG("[configuration.c] mtd: is not a valid device error when saving configuration\n");
        return false;
        break;
    case -ENOTSUP:
        DEBUG("[configuration.c] mtd: operation is not supported when saving configuration\n");
        return false;
        break;
    case -EOVERFLOW:
        DEBUG("[configuration.c] mtd: overflow error when saving configuration\n");
        return false;
        break;
    case -EINVAL:
        DEBUG("[configuration.c] mtd: invalid argument error when saving configuration\n");
        return false;
        break;
    case -EIO:
        DEBUG("[configuration.c] mtd: I/O error when saving configuration\n");
        return false;
        break;
    default:
        if (res < 0)
        {
            DEBUG("[configuration.c] mtd: Unknown error when saving configuration. Error code: %d\n", res);
            return false;
        }
    }
    return true;
}

bool configuration_load(TerraConfiguration *config, semtech_loramac_t *loramac_config)
{
    // this function loads the configuration from the mtd flash

    // read mtd flash page and handle any return errors. we early exit if errors
    int res = mtd_read_page(CONFIGURATION_MTD_DEVICE, config_buffer, CONFIGURATION_MTD_SECTOR * CONFIGURATION_MTD_DEVICE->pages_per_sector, 0, sizeof(config_buffer));
    switch (res)
    {
    case -ENODEV:
        DEBUG("[configuration.c] mtd: is not a valid device error when loading configuration\n");
        return false;
        break;
    case -ENOTSUP:
        DEBUG("[configuration.c] mtd: operation is not supported when loading configuration\n");
        return false;
        break;
    case -EOVERFLOW:
        DEBUG("[configuration.c] mtd: overflow error when loading configuration\n");
        return false;
        break;
    case -EIO:
        DEBUG("[configuration.c] mtd: I/O error when loading configuration\n");
        return false;
        break;
    default:
        if (res < 0)
        {
            DEBUG("[configuration.c] mtd: Unknown error when loading configuration\n");
            return false;
        }
    }
    uint8_t* config_bufferPtr = config_buffer;
    // verify magic word
    if (memcmp(config_bufferPtr, CONFIGURATION_MAGIC, CONFIGURATION_MAGIC_SIZE) != 0)
    {
        DEBUG("[configuration.c] Failed to verify magic word\n");
        return false;
    }
    config_bufferPtr += CONFIGURATION_MAGIC_SIZE;
    // copy the buffer into the config structs
    // first the LoRaWAN Config

    semtech_loramac_set_deveui(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_DEVEUI_LEN;

    semtech_loramac_set_appeui(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPEUI_LEN;

    semtech_loramac_set_appkey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPKEY_LEN;

    semtech_loramac_set_nwkskey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_NWKSKEY_LEN;

    semtech_loramac_set_appskey(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_APPSKEY_LEN;

    semtech_loramac_set_devaddr(loramac_config, config_bufferPtr);
    config_bufferPtr += LORAMAC_DEVADDR_LEN;

    uint32_t rx2_freq = (uint32_t) (*config_bufferPtr) << 24 | 
        (uint32_t) (*(config_bufferPtr+1)) << 16 |
        (uint32_t) (*(config_bufferPtr+2)) << 8  |
        (uint32_t) (*(config_bufferPtr+3));

    semtech_loramac_set_rx2_freq(loramac_config, rx2_freq); //TODO: this is being set or read wrong
    config_bufferPtr += LORAMAC_RX2_FREQ_LEN;

    semtech_loramac_set_rx2_dr(loramac_config, *config_bufferPtr);
    config_bufferPtr += LORAMAC_RX2_DR_LEN;

    semtech_loramac_set_join_state(loramac_config, config_bufferPtr);
    config_bufferPtr += sizeof(bool);

    //then the terra configuration
    config->loop_counter = (
        ((uint32_t) (*config_bufferPtr)) << 24 | 
        ((uint32_t) (*(config_bufferPtr+1))) << 16 |
        ((uint32_t) (*(config_bufferPtr+2))) << 8  |
        ((uint32_t) (*(config_bufferPtr+3)))
        );

    config_bufferPtr += CONFIGURATION_LOOP_COUNTER_SIZE;
    
    config->raw_message_size = *config_bufferPtr++;

    memcpy(config->raw_message_buffer, config_bufferPtr, config->raw_message_size);
    config_bufferPtr += config->raw_message_size;
    memcpy(config->window_data, config_bufferPtr, sizeof(config->window_data));


    return true;
}

#else
#error "no storage area defined!"
#endif