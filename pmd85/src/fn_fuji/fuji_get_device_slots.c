#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_device_slots(DeviceSlot *d, size_t size)
{
    struct _gds
    {
        uint8_t opcode;
        uint8_t cmd;
    } gds;

    gds.opcode = OP_FUJI;
    gds.cmd = FUJICMD_READ_DEVICE_SLOTS;

    bus_ready();

    dwwrite((uint8_t *)&gds, sizeof(gds));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)d, sizeof(DeviceSlot) * size);
}
