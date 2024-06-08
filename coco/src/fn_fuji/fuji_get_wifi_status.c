#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

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
    dwread((uint8_t *)status, sizeof(uint8_t));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
