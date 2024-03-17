#include <stdint.h>

#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

uint8_t fn_io_base64_encode_compute()
{
    // send CTRL command: 0xCF
    // PAYLOAD: N/A

    return sp_control(0, 0xCF);
}
