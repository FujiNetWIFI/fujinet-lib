#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_get_host_slots(HostSlot *h, size_t size)
{
    struct _ghs
    {
        uint8_t opcode;
        uint8_t cmd;
    } ghs;

    ghs.opcode = OP_FUJI;
    ghs.cmd = FUJICMD_READ_HOST_SLOTS;

    bus_ready();
    dwwrite((uint8_t *)&ghs, sizeof(ghs));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)h, sizeof(HostSlot) * size);
}
