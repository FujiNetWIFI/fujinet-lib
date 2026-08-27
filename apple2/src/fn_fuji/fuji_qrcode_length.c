#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
    if (sp_get_fuji_id() == 0) {
        return false;
    }

    // CONTROL carries output_mode; QRMixin re-renders and queues the 4-byte
    // length. SmartPort can't return payload on a CONTROL, so a following
    // STATUS drains the queued response.
    sp_payload[0] = 1;
    sp_payload[1] = 0;
    sp_payload[2] = output_mode;

    sp_error = sp_control(sp_fuji_id, FUJICMD_QRCODE_LENGTH);
    if (sp_error != 0) {
        return false;
    }

    sp_error = sp_status(sp_fuji_id, FUJICMD_QRCODE_LENGTH);
    if (sp_error != 0) {
        return false;
    }

    *len = (unsigned long)sp_payload[0]
         | ((unsigned long)sp_payload[1] << 8)
         | ((unsigned long)sp_payload[2] << 16)
         | ((unsigned long)sp_payload[3] << 24);
    return true;
}
