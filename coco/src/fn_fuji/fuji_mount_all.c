#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_mount_all()
{
    struct _ma
    {
        uint8_t opcode;
        uint8_t cmd;
    } ma;

    ma.opcode = OP_FUJI;
    ma.cmd = FUJICMD_MOUNT_ALL;

    bus_ready();
    dwwrite((uint8_t *)&ma, sizeof(ma));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
