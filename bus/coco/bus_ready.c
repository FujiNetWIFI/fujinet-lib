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

void bus_ready(void)
{
    struct _readycmd
    {
        uint8_t opcode;
        uint8_t command;
    } rc;

    uint8_t r, timeouts;

    rc.opcode = OP_FUJI;
    rc.command = FUJICMD_DEVICE_READY;

    for (;;)
    {
        dwwrite((uint8_t *)&rc,sizeof(rc));
        for (timeouts = 0; timeouts < READY_RESEND_TIMEOUTS; timeouts++)
            if (dwread((uint8_t *)&r,sizeof(r)))
                return;
    }
}
