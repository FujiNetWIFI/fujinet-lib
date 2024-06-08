#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_decode_input(char *s, uint16_t len)
{
    struct _bdi
    {
        uint8_t opcode;
        uint8_t cmd;
    } bdi;

    bdi.opcode = OP_FUJI;
    bdi.cmd = FUJICMD_BASE64_DECODE_INPUT;
    
    bus_ready();
    dwwrite((uint8_t *)&bdi, sizeof(bdi));
    dwwrite((uint8_t *)s,len);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
