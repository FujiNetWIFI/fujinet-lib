#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-coco.h>

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
    // DriveWire reads 16 bit parameters big-endian, which is already the 6809's
    // byte order, so no swap here (unlike the little-endian pmd85 port).
    qi.len = len;

    bus_ready();

    dwwrite((uint8_t *)&qi, sizeof(qi));
    dwwrite((uint8_t *)s, len);

    return !fuji_get_error();
}
