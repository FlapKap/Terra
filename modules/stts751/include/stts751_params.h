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
 * @name    Set default configuration parameters
 * @{
 */
#ifndef STTS751_PARAM_PARAM1
#define STTS751_PARAM_PARAM1
#endif

#ifndef STTS751_PARAMS
#define STTS751_PARAMS
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
