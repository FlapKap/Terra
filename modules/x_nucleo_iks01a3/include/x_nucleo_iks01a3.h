/*
 * Copyright (C) 2023 Kasper Hjort Berthelsen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_st_x-nucleo-iks01a3 ST X-NUCLEO-IKS01A3
 * @ingroup     drivers_misc
 * @brief       Driver for the ST X-NUCLEO-IKS01A3 sensor shield
 *
 * @{
 *
 * @file
 *
 * @author      Kasper Hjort Berthelsen <khjo@itu.dk>
 */

#ifndef X_NUCLEO_IKS01A3_H
#define X_NUCLEO_IKS01A3_H

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
    } x_nucleo_iks01a3_params_t;

    /**
     * @brief   Device descriptor for the driver
     */
    typedef struct
    {
        /** Device initialization parameters */
        x_nucleo_iks01a3_params_t params;
    } x_nucleo_iks01a3_t;

    /**
     * @brief   Initialize the given device
     *
     * @param[inout] dev        Device descriptor of the driver
     * @param[in]    params     Initialization parameters
     *
     * @return                  0 on success
     */
    int x_nucleo_iks01a3_init(x_nucleo_iks01a3_t *dev, const x_nucleo_iks01a3_params_t *params);

#ifdef __cplusplus
}
#endif

#endif /* X_NUCLEO_IKS01A3_H */
/** @} */
