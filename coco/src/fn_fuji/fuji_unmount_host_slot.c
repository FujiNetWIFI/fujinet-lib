#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_unmount_host_slot(uint8_t hs)
{
    struct _uhs
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t hs;
    } uhs;

    uhs.opcode = OP_FUJI;
    uhs.cmd = FUJICMD_MOUNT_HOST;
    uhs.hs = hs;

    bus_ready();
    dwwrite((uint8_t *)&uhs, sizeof(uhs));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
