#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_input(char *s, uint16_t len)
{
    if (sp_get_fuji_id() == 0) {
        return false;
    }

    if (len + 2 > MAX_DATA_LEN) {
        return false;
    }

    // QRMixin reads param(0) = length (uint16), then the payload bytes.
    sp_payload[0] = (len + 2) & 0xFF;
    sp_payload[1] = (len + 2) >> 8;
    sp_payload[2] = len & 0xFF;
    sp_payload[3] = len >> 8;
    memcpy(&sp_payload[4], s, len);

    sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_INPUT);
    return sp_error == 0;
}
