/**
 * @brief   Bus ready?
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Block until device ready.
 */

#include "dw.h"
#include "fujinet-fuji-coco.h"
#include <fujinet-commands.h>

/* DWRead masks IRQs, so no clock runs while it waits: count its timeouts.
 * 20 is about 14 s on a 1.79 MHz bit-banger; every resend queues a reply. */
#define READY_RESEND_TIMEOUTS 20

static const uint8_t ready_cmd[] = { OP_FUJI, FUJICMD_DEVICE_READY };

void bus_ready(void)
{
    uint8_t r, timeouts = 0;

    do
    {
        if (!timeouts--)
        {
            dwwrite((uint8_t *)ready_cmd,sizeof(ready_cmd));
            timeouts = READY_RESEND_TIMEOUTS - 1;
        }
    } while (!dwread(&r,sizeof(r)));
}
