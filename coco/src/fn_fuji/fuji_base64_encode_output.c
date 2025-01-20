#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_encode_output(char *s, uint16_t len)
{
    struct _beo
    {
        uint8_t opcode;
        uint8_t cmd;
    } beo;

    beo.opcode = OP_FUJI;
    beo.cmd = FUJICMD_BASE64_ENCODE_OUTPUT;

    bus_ready();

    dwwrite((uint8_t *)&beo, sizeof(beo));
    fuji_get_response((uint8_t *)s, len);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
