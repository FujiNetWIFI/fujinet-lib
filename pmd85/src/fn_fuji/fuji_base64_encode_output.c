#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

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
    if (fuji_get_error())
        return false;
    
    return fuji_get_response((uint8_t *)s, len);
}
