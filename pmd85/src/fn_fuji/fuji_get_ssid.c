#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_ssid(NetConfig *net_config)
{
    struct _gs
    {
        uint8_t opcode;
        uint8_t cmd;
    } gs;

    gs.opcode = OP_FUJI;
    gs.cmd = FUJICMD_GET_SSID;

    bus_ready();

    dwwrite((uint8_t *)&gs, sizeof(gs));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)net_config, sizeof(NetConfig));
}
