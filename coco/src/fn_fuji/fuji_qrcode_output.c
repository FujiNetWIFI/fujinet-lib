#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

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
    // Unlike base64's output, this command does take a byte count parameter,
    // so it must go on the wire. Big-endian, which the 6809 already is.
    qo.len = len;

    bus_ready();

    dwwrite((uint8_t *)&qo, sizeof(qo));
    if (fuji_get_error())
        return false;

    return fuji_get_response((uint8_t *)s, len);
}
