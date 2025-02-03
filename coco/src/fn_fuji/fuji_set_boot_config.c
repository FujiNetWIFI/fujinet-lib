#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_set_boot_config(uint8_t toggle)
{
    struct _sbc
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t toggle;
    } sbc;

    sbc.opcode = OP_FUJI;
    sbc.cmd = FUJICMD_CONFIG_BOOT;
    sbc.toggle = toggle;

    bus_ready();
    dwwrite((uint8_t *)&sbc, sizeof(sbc));
    
    return !fuji_get_error();
}
