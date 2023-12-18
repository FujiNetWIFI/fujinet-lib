#include <stdint.h>
#include <string.h>

#include "fujinet-io.h"
#include "fujinet-network-apple2.h"

uint8_t fn_io_base64_encode_input(char *s, uint16_t len)
{
    // send CTRL command: 0xD0
    // PAYLOAD:
    // 0,1 : length
    // 2+  : string

    if (len > MAX_SP_PAYLOAD) return 1;
    strncpy(sp_payload, s, len);
    return sp_control(0, 0xD0);

}
