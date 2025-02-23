#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
    struct _phs
    {
        uint8_t opcode;
        uint8_t cmd;
    } phs;

    phs.opcode = OP_FUJI;
    phs.cmd = FUJICMD_WRITE_HOST_SLOTS;

    bus_ready();
    dwwrite((uint8_t *)&phs, sizeof(phs));
    dwwrite((uint8_t *)h, sizeof(HostSlot) * size);
    
    return !fuji_get_error();
}
