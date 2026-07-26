#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
    if (sp_get_fuji_id() == 0) {
        return false;
    }

    sp_payload[0] = 3;
    sp_payload[1] = 0;
    sp_payload[2] = version;
    sp_payload[3] = ecc;
    sp_payload[4] = shorten;

    sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_ENCODE);
    return sp_error == 0;
}
