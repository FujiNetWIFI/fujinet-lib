#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_base64_encode_input(char *s, uint16_t len)
{
    struct _bei
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t len;
    } bei;

    bei.opcode = OP_FUJI;
    bei.cmd = FUJICMD_BASE64_ENCODE_INPUT;
    bei.len = (len << 8) | (len >> 8);

    bus_ready();

    dwwrite((uint8_t *)&bei, sizeof(bei));
    dwwrite((uint8_t *)s, len);
    
    return !fuji_get_error();
}
