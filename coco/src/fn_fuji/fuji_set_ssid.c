#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_ssid(NetConfig *nc)
{
    struct _ss
    {
        uint8_t opcode;
        uint8_t cmd;
    } ss;

    ss.opcode = OP_FUJI;
    ss.cmd = FUJICMD_SET_SSID;
    
    bus_ready();
    dwwrite((uint8_t *)&ss, sizeof(ss));
    dwwrite((uint8_t *)&nc, sizeof(NetConfig));
    
    return !fuji_get_error();
}
