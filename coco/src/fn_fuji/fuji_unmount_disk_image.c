#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_unmount_disk_image(uint8_t ds)
{
    struct _udi
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t ds;
    } udi;

    udi.opcode = OP_FUJI;
    udi.cmd = FUJICMD_UNMOUNT_IMAGE;
    udi.ds = ds;

    bus_ready();
    dwwrite((uint8_t *)&udi, sizeof(udi));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
