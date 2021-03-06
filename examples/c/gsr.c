/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <signal.h>

#include "gsr.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]

    // Instantiate a gsr sensor on analog pin A0
    gsr_context sensor = gsr_init(0);

    if (!sensor)
    {
        printf("gsr_init() failed.\n");
        return -1;
    }

    // Set the aref, scale, and offset
    gsr_set_aref(sensor, 5.0);
    gsr_set_scale(sensor, 1.0);
    gsr_set_offset(sensor, -.1);
    printf("aRef: %0.03f scale: %0.03f offset: %0.03f\n\n",
            gsr_get_aref(sensor),
            gsr_get_scale(sensor),
            gsr_get_offset(sensor));

    // Every half a second, sample the sensor output
    while (shouldRun)
    {
        float normalized = 0.0;
        float raw_volts = 0.0;
        float volts = 0.0;

        gsr_get_normalized(sensor, &normalized);
        gsr_get_raw_volts(sensor, &raw_volts);
        gsr_get_volts(sensor, &volts);

        printf("Normalized output: %0.03f, raw gsr sensor output: %0.03f v "
                "adjusted output: %0.03f v\n", normalized, raw_volts, volts);

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    gsr_close(sensor);

    return 0;
}
