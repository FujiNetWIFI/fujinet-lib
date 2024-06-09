#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
    struct _phs
    {
        uint8_t opcode;
        uint8_t cmd;
    } phs;

    bus_ready();
    dwwrite((uint8_t *)&phs, sizeof(phs));
    dwwrite((uint8_t *)h, size);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
