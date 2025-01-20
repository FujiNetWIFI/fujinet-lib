#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_decode_output(char *s, uint16_t len)
{
    struct _bdo
    {
        uint8_t opcode;
        uint8_t cmd;
    } bdo;

    bdo.opcode = OP_FUJI;
    bdo.cmd = FUJICMD_BASE64_DECODE_OUTPUT;
    
    bus_ready();

    dwwrite((uint8_t *)&bdo, sizeof(bdo));
    fuji_get_response((uint8_t *)s, len);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
