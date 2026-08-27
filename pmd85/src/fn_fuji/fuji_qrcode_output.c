#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_qrcode_output(char *s, uint16_t len)
{
    struct _qo
    {
        uint8_t opcode;
        uint8_t cmd;
        uint16_t len;
    } qo;

    qo.opcode = OP_FUJI;
    qo.cmd = FUJICMD_QRCODE_OUTPUT;
    qo.len = (len << 8) | (len >> 8);

    bus_ready();
    dwwrite((uint8_t *)&qo, sizeof(qo));
    if (fuji_get_error())
        return false;

    return fuji_get_response((uint8_t *)s, len);
}
