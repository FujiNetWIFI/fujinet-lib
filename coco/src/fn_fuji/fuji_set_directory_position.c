#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_directory_position(uint16_t pos)
{
    struct _sdp
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t pos;
    } sdp;

    sdp.opcode = OP_FUJI;
    sdp.cmd = FUJICMD_SET_DIRECTORY_POSITION;
    sdp.pos = pos;

    bus_ready();

    dwwrite((uint8_t *)&sdp, sizeof(sdp));
    if (fuji_get_error())
        return false;
    
    return true;
}