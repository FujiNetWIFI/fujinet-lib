#include <stdint.h>

#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

uint8_t fn_io_base64_encode_length(unsigned long *len)
{
    // send STATUS command: 0xCE
    // Return has length in first 2 bytes of payload

    // TODO: how do we detect an error status coming back rather than data returned?

    uint16_t length = sp_payload[0] | (sp_payload[1] << 8);
    *len = length;

    return 0;
}
