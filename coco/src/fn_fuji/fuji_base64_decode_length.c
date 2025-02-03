#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_decode_length(unsigned long *len)
{
    struct _bdl
    {
        uint8_t opcode;
        uint8_t cmd;
    } bdl;

    bdl.opcode = OP_FUJI;
    bdl.cmd = FUJICMD_BASE64_DECODE_LENGTH;

    bus_ready();
    
    dwwrite((uint8_t *)&bdl, sizeof(bdl));
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)len, sizeof(unsigned long));
}
