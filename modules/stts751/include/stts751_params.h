/*
 * Copyright (C) 2023 Kasper Hjort Berthelsen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_st_stts751
 *
 * @{
 * @file
 * @brief       Default configuration
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef STTS751_PARAMS_H
#define STTS751_PARAMS_H

#include "board.h"
#include "stts751.h"
#include "stts751_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the STTS751 driver
 * @{
 */
#ifndef STTS751_PARAM_I2C
#define STTS751_PARAM_I2C I2C_DEV(0)
#endif

#ifndef STTS751_PARAM_ADDR
#define STTS751_PARAM_ADDR (STTS751_I2C_ADDRESS)
#endif

#ifndef STTS751_PARAM_CONVERSION_RATE
#define STTS751_PARAM_CONVERSION_RATE (STTS751_DEFAULT_CONVERSION_RATE)
#endif

#ifndef STTS751_PARAM_TEMP_HIGH_LIMIT_HIGH_BYTE
#define STTS751_PARAM_TEMP_HIGH_LIMIT_HIGH_BYTE (STTS751_DEFAULT_TEMP_HIGH_LIMIT_HIGH_BYTE)
#endif

#ifndef STTS751_PARAM_TEMP_HIGH_LIMIT_LOW_BYTE
#define STTS751_PARAM_TEMP_HIGH_LIMIT_LOW_BYTE (STTS751_DEFAULT_TEMP_HIGH_LIMIT_LOW_BYTE)
#endif

#ifndef STTS751_PARAM_TEMP_LOW_LIMIT_HIGH_BYTE
#define STTS751_PARAM_TEMP_LOW_LIMIT_HIGH_BYTE (STTS751_DEFAULT_TEMP_LOW_LIMIT_HIGH_BYTE)
#endif

#ifndef STTS751_PARAM_TEMP_LOW_LIMIT_LOW_BYTE
#define STTS751_PARAM_TEMP_LOW_LIMIT_LOW_BYTE (STTS751_DEFAULT_TEMP_LOW_LIMIT_LOW_BYTE)
#endif

#ifndef STTS751_PARAM_THERM_LIMIT
#define STTS751_PARAM_THERM_LIMIT (STTS751_DEFAULT_THERM_LIMIT)
#endif

#ifndef STTS751_PARAM_THERM_HYSTERESIS
#define STTS751_PARAM_THERM_HYSTERESIS (STTS751_DEFAULT_THERM_HYSTERESIS)
#endif

#ifndef STTS751_SMBUS_TIMEOUT_ENABLE
#define STTS751_SMBUS_TIMEOUT_ENABLE (STTS751_DEFAULT_SMBUS_TIMEOUT_ENABLE)
#endif

#ifndef STTS751_PARAMS
#define STTS751_PARAMS {                                        \
    .i2c = STTS751_PARAM_I2C,                                   \
    .addr = STTS751_PARAM_ADDR,                                 \
    .conversion_rate = STTS751_PARAM_CONVERSION_RATE,           \
    .temp_high_limit = {                                        \
        .high_byte = STTS751_PARAM_TEMP_HIGH_LIMIT_HIGH_BYTE,   \
        .low_byte = STTS751_PARAM_TEMP_HIGH_LIMIT_LOW_BYTE      \
    },                                                          \
    .temp_low_limit = {                                         \
        .high_byte = STTS751_PARAM_TEMP_LOW_LIMIT_HIGH_BYTE,    \
        .low_byte = STTS751_PARAM_TEMP_LOW_LIMIT_LOW_BYTE       \
    },                                                          \
    .therm_limit = STTS751_PARAM_THERM_LIMIT,                   \
    .therm_hysteresis = STTS751_PARAM_THERM_HYSTERESIS,         \
    .smbus_timeout_enable = STTS751_SMBUS_TIMEOUT_ENABLE        \
    }
#endif

#ifndef STTS751_SAULINFO
#define STTS751_SAULINFO { .name = "stts751"}
#endif

/**@}*/

/**
 * @brief   Configuration struct
 */
static const stts751_params_t   stts751_params[] =
{
    STTS751_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* STTS751_PARAMS_H */
/** @} */
