#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
{
    struct _gace
    {
        uint8_t opcode;
        uint8_t cmd;
    } gace;

    bus_ready();

    dwwrite((uint8_t *)&gace, sizeof(gace));
    dwread((uint8_t *)ac, sizeof(AdapterConfigExtended));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
