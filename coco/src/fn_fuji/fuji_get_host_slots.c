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

    bus_get_response(OP_FUJI,(uint8_t *)h, sizeof(HostSlot) * size);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
