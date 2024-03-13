#include "configuration.h"

#ifndef DISABLE_LORA
#include "lorawan.h"
#include "semtech_loramac.h"
#endif

#define ENABLE_DEBUG 1
#include "debug.h"

#include "assert.h"

#ifdef MODULE_EEPREG
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

bool configuration_save(TerraConfiguration *config, semtech_loramac_t *loramac)
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

bool configuration_load(TerraConfiguration *config, semtech_loramac_t *loramac)
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

typedef struct _loramac_settings
{
    uint8_t deveui[LORAMAC_DEVEUI_LEN];   /**< device EUI */
    uint8_t appeui[LORAMAC_APPEUI_LEN];   /**< application EUI */
    uint8_t appkey[LORAMAC_APPKEY_LEN];   /**< application key */
    uint8_t nwkskey[LORAMAC_NWKSKEY_LEN]; /**< network session key */
    uint8_t appskey[LORAMAC_APPSKEY_LEN]; /**< application session key */
    uint8_t devaddr[LORAMAC_DEVADDR_LEN]; /**< device address */
    uint32_t rx2_freq;
    uint8_t rx2_dr;
    bool joined;
} loramac_settings_t;

static loramac_settings_t loramac_settings;
static char config_buffer[CONFIGURATION_MAGIC_SIZE + sizeof(TerraConfiguration) + sizeof(loramac_settings_t)] __attribute__((aligned(FLASHPAGE_WRITE_BLOCK_ALIGNMENT)));

static inline unsigned int _get_number_of_sectors_for_buffer(void)
{
    unsigned int num_sectors = sizeof(config_buffer) / (CONFIGURATION_MTD_DEVICE->pages_per_sector * CONFIGURATION_MTD_DEVICE->page_size);
    assert(num_sectors > 0 && num_sectors <= CONFIGURATION_MTD_DEVICE->sector_count);
    printf("num_sectors: %d\n", num_sectors);
    return num_sectors;
}

/**
 * @brief
 * @param config
 * @param loramac_config
 * @return true if the save was successful. False otherwise
 * @note this saves to flash. This causes wear so use it as little as possible
 */
bool configuration_save(TerraConfiguration *config, semtech_loramac_t *loramac_config)
{
    static_assert(sizeof(*config) == sizeof(TerraConfiguration), "config doesnt have type of TerraConfiguration"); // just sanitycheck that if we change the type of config we also need to change the type of config_buffer
    // take both app config and loramac config and write to mtd flash. Need to take care of flash page boundaries. Possibly also the erase state of bytes

    // assert we can fit the buffer within a single sector
    assert(sizeof(config_buffer) < CONFIGURATION_MTD_DEVICE->sector_count * CONFIGURATION_MTD_DEVICE->pages_per_sector * CONFIGURATION_MTD_DEVICE->page_size);
    assert(CONFIGURATION_MTD_DEVICE->sector_count > CONFIGURATION_MTD_SECTOR);

    // create the buffer. Make sure its aligned

    // create the loramac_settings struct

    semtech_loramac_get_deveui(loramac_config, loramac_settings.deveui);
    semtech_loramac_get_appeui(loramac_config, loramac_settings.appeui);
    semtech_loramac_get_appkey(loramac_config, loramac_settings.appkey);
    semtech_loramac_get_nwkskey(loramac_config, loramac_settings.nwkskey);
    semtech_loramac_get_appskey(loramac_config, loramac_settings.appskey);
    semtech_loramac_get_devaddr(loramac_config, loramac_settings.devaddr);
    loramac_settings.rx2_freq = semtech_loramac_get_rx2_freq(loramac_config);
    loramac_settings.rx2_dr = semtech_loramac_get_rx2_dr(loramac_config);
    loramac_settings.joined = semtech_loramac_get_join_state(loramac_config);

    // copy magic word into buffer
    if (strcpy(config_buffer, CONFIGURATION_MAGIC) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy magic word to buffer\n");
        return false;
    };

    // copy terraconfig into the buffer
    if (memcpy(&config_buffer[CONFIGURATION_MAGIC_SIZE], config, sizeof(*config)) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy terraconfig to buffer\n");
        return false;
    };
    // copy loramac settings into the buffer
    if (memcpy(&config_buffer[sizeof(*config)], &loramac_settings, sizeof(loramac_settings)) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy loramac settings to buffer\n");
        return false;
    };
    // write the buffer and handle any return errors
    int res = mtd_write_sector(CONFIGURATION_MTD_DEVICE, config_buffer, CONFIGURATION_MTD_SECTOR, _get_number_of_sectors_for_buffer());
    switch (res)
    {
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
        else if (res != sizeof(config_buffer))
        {
            DEBUG("[configuration.c] mtd: Partial write error when saving configuration. saved %d bytes instead of %d\n", res, sizeof(config_buffer));
            return false;
        }
        else
        {
            DEBUG("[configuration.c] mtd: Configuration saved successfully\n");
            return true;
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
        else if (res != sizeof(config_buffer))
        {
            DEBUG("[configuration.c] mtd: read of configuration was incomplete. Read %" PRIiLEAST8 " bytes instead of %" PRIxSIZE "\n", res, sizeof(config_buffer));
            return false;
        }
    }

    // verify magic word
    if (memcmp(config_buffer, CONFIGURATION_MAGIC, CONFIGURATION_MAGIC_SIZE) != 0)
    {
        DEBUG("[configuration.c] Failed to verify magic word\n");
        return false;
    }

    // copy the buffer into the config structs
    // first the TerraConfiguration
    if (memcpy(config, config_buffer, sizeof(*config)) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy buffer to terraconfig\n");
        return false;
    };

    // Then the loramac settings
    if (memcpy(&loramac_settings, &config_buffer[sizeof(*config)], sizeof(loramac_settings)) == NULL)
    {
        DEBUG("[configuration.c] Failed to copy buffer to loramac settings\n");
        return false;
    };

    // Use the loramac settings to fill out the semtech_loramac conf
    semtech_loramac_set_deveui(loramac_config, loramac_settings.deveui);
    semtech_loramac_set_appeui(loramac_config, loramac_settings.appeui);
    semtech_loramac_set_appkey(loramac_config, loramac_settings.appkey);
    semtech_loramac_set_nwkskey(loramac_config, loramac_settings.nwkskey);
    semtech_loramac_set_appskey(loramac_config, loramac_settings.appskey);
    semtech_loramac_set_devaddr(loramac_config, loramac_settings.devaddr);
    semtech_loramac_set_rx2_dr(loramac_config, loramac_settings.rx2_dr);
    semtech_loramac_set_join_state(loramac_config, loramac_settings.joined);
    semtech_loramac_set_rx2_freq(loramac_config, loramac_settings.rx2_freq);

    return true;
}

#else
#error "no storage area defined!"
#endif