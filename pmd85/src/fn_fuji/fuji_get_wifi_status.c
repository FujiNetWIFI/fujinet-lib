#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_wifi_status(uint8_t *status)
{
    struct _gws
    {
        uint8_t opcode;
        uint8_t cmd;
    } gws;

    gws.opcode = OP_FUJI;
    gws.cmd = FUJICMD_GET_WIFISTATUS;

    bus_ready();

    dwwrite((uint8_t *)&gws, sizeof(gws));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)status, sizeof(uint8_t));
}
