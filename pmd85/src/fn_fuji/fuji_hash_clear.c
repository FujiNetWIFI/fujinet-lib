#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_hash_clear()
{
    struct _hc
    {
        uint8_t opcode;
        uint8_t cmd;
    } hc;

    hc.opcode = OP_FUJI;
    hc.cmd = FUJICMD_HASH_CLEAR;
    
    bus_ready();
    dwwrite((uint8_t *)&hc, sizeof(hc));
    
    return !fuji_get_error();
}
