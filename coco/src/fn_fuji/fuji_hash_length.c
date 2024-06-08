#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_hash_length(uint8_t mode)
{
    uint8_t len = 0;
    
    struct _hl
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t mode;
    } hl;

    hl.opcode = OP_FUJI;
    hl.cmd = FUJICMD_HASH_LENGTH;
    hl.mode = mode;

    bus_ready();

    dwwrite((uint8_t *)&hl, sizeof(hl));
    bus_get_response(OP_FUJI,&len, sizeof(uint8_t));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
