#include <stdint.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_output(char *s, uint16_t len)
{
    if (sp_get_fuji_id() == 0) {
        return false;
    }

    // CONTROL carries the requested length; QRMixin queues that many bytes.
    // A following STATUS drains the queued response.
    sp_payload[0] = 2;
    sp_payload[1] = 0;
    sp_payload[2] = len & 0xFF;
    sp_payload[3] = len >> 8;

    sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_OUTPUT);
    if (sp_error != 0) {
        return false;
    }

    sp_error = sp_status(sp_fuji_id, FUJICMD_QRCODE_OUTPUT);
    if (sp_error != 0) {
        return false;
    }

    memcpy(s, &sp_payload[0], len);
    return true;
}
