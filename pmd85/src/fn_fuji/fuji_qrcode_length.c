#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
    struct _ql
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t output_mode;
    } ql;

    ql.opcode = OP_FUJI;
    ql.cmd = FUJICMD_QRCODE_LENGTH;
    ql.output_mode = output_mode;

    bus_ready();
    dwwrite((uint8_t *)&ql, sizeof(ql));
    if (fuji_get_error())
        return false;

    return fuji_get_response((uint8_t *)len, sizeof(unsigned long));
}
