#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_close_directory()
{
    struct _cd
    {
        uint8_t opcode;
        uint8_t cmd;
    } cd;

    cd.opcode = OP_FUJI;
    cd.cmd = FUJICMD_CLOSE_DIRECTORY;
    
    bus_ready();

    dwwrite((uint8_t *)&cd, sizeof(cd));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
