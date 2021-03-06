/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>
#include <mraa/aio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lm35.h
     * @library lm35
     * @brief C API for the LM35 Temperature Sensor
     *
     * @include lm35.c
     */

    /**
     * device context
     */
    typedef struct _lm35_context
    {
        mraa_aio_context    aio;

        // ADC reference voltage
        float               aref;
        // ADC resolution
        float               ares;

        // scale and offset
        float               scale;
        float               offset;
    } *lm35_context;

    /**
     * LM35 initialization.
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage
     */
    lm35_context lm35_init(int pin, float aref);

    /**
     * LM35 close.
     */
    void lm35_close(lm35_context dev);

    /**
     * Returns the temperature in degrees Celsius
     *
     * @param temperature A pointer to a float that will contain the
     * measured temperature.
     * @return UPM status
     */
    upm_result_t lm35_get_temperature(const lm35_context dev,
                                      float *temperature);

    /**
     * Set sensor offset.  This offset is applied to the return values
     * before scaling.  Default is 0.0.
     *
     * @param dev sensor context pointer
     * @param offset Offset to apply.
     */
    void lm35_set_offset(const lm35_context dev, float offset);

    /**
     * Set sensor scale.  This scale is applied to the return values
     * before the offset is applied.  Default is 1.0.
     *
     * @param dev sensor context pointer
     * @param scale Scale to apply.
     */
    void lm35_set_scale(const lm35_context dev, float scale);


#ifdef __cplusplus
}
#endif
