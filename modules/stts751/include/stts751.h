/*
 * Copyright (C) 2023 Kasper Hjort Berthelsen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_stts751 ST STTS751
 * @ingroup     drivers_sensors
 * @brief       Driver for the ST STTS751 digital temperature sensor
 *
 * @{
 *
 * @file
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef STTS751_H
#define STTS751_H

/* Add header includes here */

#ifdef __cplusplus
extern "C"
{
#endif

    /* Declare the API of the driver */

    /**
     * @brief   Device initialization parameters
     */
    typedef struct
    {
        /* add initialization params here */
    } stts751_params_t;

    /**
     * @brief   Device descriptor for the driver
     */
    typedef struct
    {
        /** Device initialization parameters */
        stts751_params_t params;
    } stts751_t;

    /**
     * @brief   Initialize the given device
     *
     * @param[inout] dev        Device descriptor of the driver
     * @param[in]    params     Initialization parameters
     *
     * @return                  0 on success
     */
    int stts751_init(stts751_t *dev, const stts751_params_t *params);

#ifdef __cplusplus
}
#endif

#endif /* STTS751_H */
/** @} */
