#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_get_device_slots(DeviceSlot *d, size_t size)
{
    struct _gds
    {
        uint8_t opcode;
        uint8_t cmd;
    } gds;

    bus_ready();

    dwwrite((uint8_t *)&gds, sizeof(gds));
    bus_get_response(OP_FUJI,(uint8_t *)d, size);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
