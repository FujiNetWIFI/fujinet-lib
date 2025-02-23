#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_base64_decode_input(char *s, uint16_t len)
{
    struct _bdi
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t len;
    } bdi;

    bdi.opcode = OP_FUJI;
    bdi.cmd = FUJICMD_BASE64_DECODE_INPUT;
    bdi.len = (len << 8) | (len >> 8);
    
    bus_ready();
    dwwrite((uint8_t *)&bdi, sizeof(bdi));
    dwwrite((uint8_t *)s,len);
    
    return !fuji_get_error();
}
