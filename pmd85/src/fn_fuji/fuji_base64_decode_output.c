#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

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
    if (fuji_get_error())
        return false;

    return fuji_get_response((uint8_t *)s, len);
}
