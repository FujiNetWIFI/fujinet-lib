#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_get_adapter_config(AdapterConfig *ac)
{
    struct _gac
    {
        uint8_t opcode;
        uint8_t cmd;
    } gac;

    gac.opcode = OP_FUJI;
    gac.cmd = FUJICMD_GET_ADAPTERCONFIG;

    bus_ready();

    dwwrite((uint8_t *)&gac, sizeof(gac));
    bus_get_response(OP_FUJI,(uint8_t *)ac, sizeof(AdapterConfig));
        
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
