#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_hash_compute_no_clear(uint8_t type)
{
    struct _hc
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t type;
    } hc;

    hc.opcode = OP_FUJI;
    hc.cmd = FUJICMD_HASH_COMPUTE_NO_CLEAR;
    hc.type = type;

    bus_ready();

    dwwrite((uint8_t *)&hc, sizeof(hc));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
