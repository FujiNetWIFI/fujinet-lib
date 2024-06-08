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

    bus_ready();
    
    dwwrite((uint8_t *)&bdl, sizeof(bdl));
    dwread((uint8_t *)len, sizeof(unsigned long));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
