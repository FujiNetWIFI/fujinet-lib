#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

bool fuji_base64_encode_input(char *s, uint16_t len)
{
    struct _bei
    {
        uint8_t opcode;
        uint8_t cmd;
    } bei;

    bei.opcode = OP_FUJI;
    bei.cmd = FUJICMD_BASE64_ENCODE_INPUT;

    bus_ready();

    dwwrite((uint8_t *)&bei, sizeof(bei));
    dwwrite((uint8_t *)s, len);
    
    return bus_error(OP_FUJI) == BUS_SUCCESS;
}
