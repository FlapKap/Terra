/*
 * Copyright (C) 2023 Kasper Hjort Berthelsen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_st_stts751
 * @{
 *
 * @file
 * @brief       Internal addresses, registers and constants
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef STTS751_CONSTANTS_H
#define STTS751_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* define here the addresses, register and constants of the driver */

/**
 * @name Addresses
 * @brief SMBus Addresses. they are selected based on the pull-up resistor value
 * of the Addr/Therm pin, and the order code of the STTS751 sensor.
 * @{
*/

#define STTS751_I2C_ADDRESS (0x94)

/** @}*/

/**
 * @name Register Map
 * @{
*/

#define STTS751_REGS_TEMP_VALUE_HIGH_BYTE       (0x00) // READ
#define STTS751_REGS_STATUS                     (0x01) // READ
#define STTS751_REGS_TEMP_VALUE_LOW_BYTE        (0x02) // READ
#define STTS751_REGS_CONFIG                     (0x03) // READ/WRITE
#define STTS751_REGS_CONVERSION_RATE            (0x04) // READ/WRITE
#define STTS751_REGS_TEMP_HIGH_LIMIT_HIGH_BYTE  (0x05) // READ/WRITE
#define STTS751_REGS_TEMP_HIGH_LIMIT_LOW_BYTE   (0x06) // READ/WRITE
#define STTS751_REGS_TEMP_LOW_LIMIT_HIGH_BYTE   (0x07) // READ/WRITE
#define STTS751_REGS_TEMP_LOW_LIMIT_LOW_BYTE    (0x08) // READ/WRITE
#define STTS751_REGS_ONE_SHOT_CONVERSION        (0x0F) // WRITE
#define STTS751_REGS_THERM_LIMIT                (0x20) // READ/WRITE
#define STTS751_REGS_THERM_HYSTERESIS           (0x21) // READ/WRITE
#define STTS751_REGS_SMBUS_TIMEOUT_ENABLE       (0x22) // READ/WRITE
#define STTS751_REGS_PRODUCT_ID                 (0xFD) // READ
#define STTS751_REGS_MANUFACTURER_ID            (0xFE) // READ
#define STTS751_REGS_REVISION_NUMBER            (0xFF) // READ

/** @}*/

/**
 * @name Default values
 * @{
*/

#define STTS751_DEFAULT_CONFIG                      (0x00)
#define STTS751_DEFAULT_CONVERSION_RATE             (1 << 2)
#define STTS751_DEFAULT_TEMP_HIGH_LIMIT_HIGH_BYTE   (0x55) // 85 Celcius
#define STTS751_DEFAULT_TEMP_HIGH_LIMIT_LOW_BYTE    (0x00)
#define STTS751_DEFAULT_TEMP_LOW_LIMIT_HIGH_BYTE    (0x00) // 0 Celcius
#define STTS751_DEFAULT_TEMP_LOW_LIMIT_LOW_BYTE     (0x00)
#define STTS751_DEFAULT_THERM_LIMIT                 (0x55) // 85 Celcius
#define STTS751_DEFAULT_THERM_HYSTERESIS            (0x0A) // 10 Celcius
#define STTS751_DEFAULT_SMBUS_TIMEOUT_ENABLE        (1 << 7)
#define STTS751_DEFAULT_PRODUCT_ID                  (0x00) // for STTS751-0. is 0x01 for STTS751-1
#define STTS751_DEFAULT_MANUFACTURER_ID             (0x53)
#define STTS751_DEFAULT_REVISION_NUMBER             (0x01)
/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* X_NUCLEO_IKS01A3_CONSTANTS_H */
/** @} */
