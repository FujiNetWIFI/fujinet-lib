#include <stdint.h>

#include "fujinet-fuji.h"
#include "fujinet-bus-apple2.h"

bool fuji_base64_encode_compute(void)
{
    // send CTRL command: FUJICMD_BASE64_ENCODE_COMPUTE
    // PAYLOAD: N/A

    return sp_control(0, FUJICMD_BASE64_ENCODE_COMPUTE) == 0;
}
