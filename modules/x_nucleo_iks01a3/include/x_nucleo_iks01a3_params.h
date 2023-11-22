/*
 * Copyright (C) 2023 Kasper Hjort Berthelsen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_st_x-nucleo-iks01a3
 *
 * @{
 * @file
 * @brief       Default configuration
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef X_NUCLEO_IKS01A3_PARAMS_H
#define X_NUCLEO_IKS01A3_PARAMS_H

#include "board.h"
#include "x_nucleo_iks01a3.h"
#include "x_nucleo_iks01a3_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters
 * @{
 */
#ifndef X_NUCLEO_IKS01A3_PARAM_PARAM1
#define X_NUCLEO_IKS01A3_PARAM_PARAM1
#endif

#ifndef X_NUCLEO_IKS01A3_PARAMS
#define X_NUCLEO_IKS01A3_PARAMS
#endif
/**@}*/

/**
 * @brief   Configuration struct
 */
static const x_nucleo_iks01a3_params_t x_nucleo_iks01a3_params[] =
{
    X_NUCLEO_IKS01A3_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* X_NUCLEO_IKS01A3_PARAMS_H */
/** @} */
