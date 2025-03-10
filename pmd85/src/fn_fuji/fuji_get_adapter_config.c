#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

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
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)ac, sizeof(AdapterConfig));
}
