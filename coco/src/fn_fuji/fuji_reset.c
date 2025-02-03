#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_reset()
{
    struct _r
    {
        uint8_t opcode;
        uint8_t cmd;
    } r;

    r.opcode = OP_FUJI;
    r.cmd = FUJICMD_RESET;
    
    bus_ready();
    dwwrite((uint8_t *)&r, sizeof(r));
    
    return !fuji_get_error();
}
