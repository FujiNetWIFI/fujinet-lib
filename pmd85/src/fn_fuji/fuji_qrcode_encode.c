#include "fujinet-fuji.h"
#include <dw.h>
#include <fujinet-fuji-pmd85.h>

bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
    struct _qe
    {
        uint8_t opcode;
        uint8_t cmd;
        uint8_t version;
        uint8_t ecc;
        uint8_t shorten;
    } qe;

    qe.opcode = OP_FUJI;
    qe.cmd = FUJICMD_QRCODE_ENCODE;
    qe.version = version;
    qe.ecc = ecc;
    qe.shorten = shorten ? 1 : 0;

    bus_ready();
    dwwrite((uint8_t *)&qe, sizeof(qe));

    return !fuji_get_error();
}
