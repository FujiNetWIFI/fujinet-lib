#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_encode_length(unsigned long *len)
{
    struct _bel
    {
        uint8_t opcode;
        uint8_t cmd;
    } bel;

    bel.opcode = OP_FUJI;
    bel.cmd = FUJICMD_BASE64_ENCODE_LENGTH;

    bus_ready();

    dwwrite((uint8_t *)&bel, sizeof(bel));
    fuji_get_response((uint8_t *)len, sizeof(unsigned long));
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
