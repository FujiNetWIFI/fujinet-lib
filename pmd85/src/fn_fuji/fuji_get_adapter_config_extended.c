#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
    struct _gace
    {
        uint8_t opcode;
        uint8_t cmd;
    } gace;

    gace.opcode = OP_FUJI;
    gace.cmd = FUJICMD_GET_ADAPTERCONFIG_EXTENDED;

    bus_ready();

    dwwrite((uint8_t *)&gace, sizeof(gace));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)ac, sizeof(AdapterConfigExtended));
}
