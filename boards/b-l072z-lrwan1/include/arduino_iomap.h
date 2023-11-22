/*
 * Copyright (C)  2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_b-l072z-lrwan1
 * @{
 *
 * @file
 * @brief       Mapping from MCU pins to Arduino pins
 *
 * You can use the defines in this file for simplified interaction with the
 * Arduino specific pin numbers.
 * 
 * Made with heavy inspiration from RIOT/boards/common/nucleo32/include/arduino_iomap.h made by Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef ARDUINO_IOMAP_H
#define ARDUINO_IOMAP_H

#include "periph/gpio.h"
#include "periph/adc.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Arduino's UART devices
 * @{
 */
#define ARDUINO_UART_D0D1       UART_DEV(0)
/** @} */

/**
 * @name    Arduino's SPI buses
 * @{
 */
/**
 * @brief   SPI_DEV(1) is connected to D11/D12/D13
 */
#if !defined(ARDUINO_SPI_D11D12D13) && defined(SPI_NUMOF)
#define ARDUINO_SPI_D11D12D13   SPI_DEV(0)
#endif
/** @} */

/**
 * @name    Arduino's I2C buses
 * @{
 */
/**
 * @brief   The first I2C bus is where shields for the Arduino UNO expect it
 */
#define ARDUINO_I2C_UNO         I2C_DEV(0)
/** @} */

/**
 * @brief   Mapping of MCU pins to Arduino pins
 * @{
 */
#define ARDUINO_PIN_0           GPIO_PIN(PORT_A, 3)
#define ARDUINO_PIN_1           GPIO_PIN(PORT_A, 2)
#define ARDUINO_PIN_2           GPIO_PIN(PORT_A, 10)
#define ARDUINO_PIN_3           GPIO_PIN(PORT_B, 13) /* Either PB13 or Not Connected. Default to PB13, due to solder bridge SB9 default being soldered */
#define ARDUINO_PIN_4           GPIO_PIN(PORT_B, 5)
#define ARDUINO_PIN_5           GPIO_PIN(PORT_B, 7)
#define ARDUINO_PIN_6           GPIO_PIN(PORT_B, 2)
#define ARDUINO_PIN_7           GPIO_PIN(PORT_A, 8)

#define ARDUINO_PIN_8           GPIO_PIN(PORT_A, 9)
#define ARDUINO_PIN_9           GPIO_PIN(PORT_B, 12)
#define ARDUINO_PIN_10          GPIO_PIN(PORT_B, 6)

#define ARDUINO_PIN_11          GPIO_PIN(PORT_B, 15)
#define ARDUINO_PIN_12          GPIO_PIN(PORT_B, 14)
#define ARDUINO_PIN_13          GPIO_PIN(PORT_A, 5) /* Either PA5 or PB13. Default to PA5, due to solder bridge SB3 default being soldered */
#define ARDUINO_PIN_14          GPIO_PIN(PORT_B, 9)
#define ARDUINO_PIN_15          GPIO_PIN(PORT_B, 8)

/* analog pins */
#define ARDUINO_PIN_A0          GPIO_PIN(PORT_A, 0)
#define ARDUINO_PIN_A1          GPIO_PIN(PORT_A, 0) /* Either PA0 or NC. Default to PA0, due to solder bridge SB7 default being soldered */
#define ARDUINO_PIN_A2          GPIO_PIN(PORT_A, 4)
//#define ARDUINO_PIN_A3          GPIO_PIN(PORT_A, 4) /* Either PA4 or NC. Default to NC, due to solder bridge SB8 default being not soldered */
#define ARDUINO_PIN_A4          GPIO_PIN(PORT_H, 1) /* Either PH0, PH1 or PB9. Default to PH1, due to solder bridge SB11 and SB1 default being not soldered */
//#define ARDUINO_PIN_A5          GPIO_PIN(PORT_C, 0) /* Either PH1, PB8 or NC. probably defaults to not connected, due to solder bridge SB10 and SB12 default being not soldered and i can't find it connected anywhere in the schematic*/

/* analog pins aliased as digital pins: */
#define ARDUINO_PIN_16          ARDUINO_PIN_A0
#define ARDUINO_PIN_17          ARDUINO_PIN_A1
#define ARDUINO_PIN_18          ARDUINO_PIN_A2
//#define ARDUINO_PIN_19          ARDUINO_PIN_A3
#define ARDUINO_PIN_20          ARDUINO_PIN_A4
//#define ARDUINO_PIN_21          ARDUINO_PIN_A5

#define ARDUINO_PIN_LAST        21
/** @} */


/**
 * @name    Mapping of Arduino analog pins to RIOT ADC lines
 * @{
 */
#define ARDUINO_A0              ADC_LINE(0)
#define ARDUINO_A1              ADC_LINE(1)
#define ARDUINO_A2              ADC_LINE(2)
#define ARDUINO_A3              ADC_LINE(3)
#define ARDUINO_A4              ADC_LINE(4)
#define ARDUINO_A5              ADC_LINE(5)

#define ARDUINO_ANALOG_PIN_LAST 5
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_IOMAP_H */
/** @} */
