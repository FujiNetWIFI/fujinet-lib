#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_put_device_slots(DeviceSlot *d, size_t size)
{
    struct _pds
    {
        uint8_t opcode;
        uint8_t cmd;
    } pds;

    pds.opcode = OP_FUJI;
    pds.cmd = FUJICMD_WRITE_DEVICE_SLOTS;

    bus_ready();
    
    dwwrite((uint8_t *)&pds, sizeof(pds));
    dwwrite((uint8_t *)d, sizeof(DeviceSlot) * size);
    
    return !fuji_get_error();
}
