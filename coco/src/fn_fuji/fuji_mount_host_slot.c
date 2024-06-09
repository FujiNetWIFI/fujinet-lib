#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_mount_host_slot(uint8_t hs)
{
    struct _mhs
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t hs;
    } mhs;

    mhs.opcode = OP_FUJI;
    mhs.cmd = FUJICMD_MOUNT_HOST;
    mhs.hs = hs;

    bus_ready();
    dwwrite((uint8_t *)&mhs, sizeof(mhs));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
