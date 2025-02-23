#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_hash_compute(uint8_t type)
{
    struct _hc
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t type;
    } hc;

    hc.opcode = OP_FUJI;
    hc.cmd = FUJICMD_HASH_COMPUTE;
    hc.type = type;

    bus_ready();

    dwwrite((uint8_t *)&hc, sizeof(hc));
    
    return !fuji_get_error();
}
