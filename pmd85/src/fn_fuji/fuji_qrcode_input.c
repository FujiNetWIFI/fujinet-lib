#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_qrcode_input(char *s, uint16_t len)
{
    struct _qi
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t len;
    } qi;

    qi.opcode = OP_FUJI;
    qi.cmd = FUJICMD_QRCODE_INPUT;
    qi.len = (len << 8) | (len >> 8);

    bus_ready();
    dwwrite((uint8_t *)&qi, sizeof(qi));
    dwwrite((uint8_t *)s, len);

    return !fuji_get_error();
}
