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
 * @brief       Device driver implementation for the ST STTS751 digital temperature sensor
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 *
 * @}
 */

#include "periph/i2c.h"

#include "stts751.h"
#include "stts751_constants.h"
#include "stts751_params.h"

#define ENABLE_DEBUG    0
#include "debug.h"

#define BUS             (dev->params.i2c)
#define ADDR            (dev->params.addr)

static uint8_t _get_config(const stts751_t *dev){
    
}

int stts751_init(stts751_t *dev, const stts751_params_t *params)
{
    dev->params = *params;

    uint8_t manu_id;
    i2c_acquire(BUS);
    /* Try to read the manufacturer ID */
    if(i2c_read_reg(BUS, ADDR, STTS751_REGS_MANUFACTURER_ID, &manu_id,0) != 0) {
        i2c_release(BUS);
        DEBUG("%s: failed to read register STTS751_REGS_MANUFACTURER_ID\n", DEBUG_FUNC);
        return -STTS751_NOBUS;
    }
    if(manu_id != STTS751_DEFAULT_MANUFACTURER_ID) {
        i2c_release(BUS);
        DEBUG("%s: invalid manufacturer ID read from STTS751\n", DEBUG_FUNC);
        return -STTS751_NODEV;
    }
    i2c_release(BUS);
}
