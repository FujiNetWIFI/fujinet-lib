#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_get_wifi_enabled()
{
    bool enabled = false;
    
    struct _gwe
    {
        uint8_t opcode;
        uint8_t cmd;
    } gwe;

    gwe.opcode = OP_FUJI;
    gwe.cmd = FUJICMD_GET_WIFI_ENABLED;

    bus_ready();

    dwwrite((uint8_t *)&gwe, sizeof(gwe));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)&enabled, sizeof(bool));
}
