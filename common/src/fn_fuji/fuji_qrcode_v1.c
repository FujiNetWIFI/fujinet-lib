#if defined(_CMOC_VERSION_)

#include <cmoc.h>
#include <coco.h>

#else

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#endif // _CMOC_VERSION_

#include "fujinet-fuji.h"

/*
 * Encode a string as a QR version 1 symbol and fetch its module matrix.
 *
 * Version 1 is asked for explicitly rather than letting the FujiNet choose.
 * With version 0 the firmware picks a version AND overrides the error
 * correction level, which is no use when the symbol has to fit a fixed display
 * area -- a version 2 symbol is 25x25 and will not fit where 21x21 does.
 *
 * The 25 character limit is the version 1 / ECC LOW alphanumeric capacity. It
 * is checked here so an over-long string fails locally and cheaply, rather than
 * after three round trips to the FujiNet.
 */

#define QRCODE_V1_MAX_INPUT 25

bool fuji_qrcode_v1(const char *s, uint8_t *out)
{
    unsigned long len = 0;
    uint16_t n;

    if (s == NULL || out == NULL)
        return false;

    n = (uint16_t) strlen(s);
    if (n == 0 || n > QRCODE_V1_MAX_INPUT)
        return false;

    if (!fuji_qrcode_input((char *) s, n))
        return false;

    if (!fuji_qrcode_encode(1, QR_ECC_LOW, false))
        return false;

    if (!fuji_qrcode_length(QR_OUTPUT_BINARY, &len))
        return false;

    /* A version 1 symbol is always 57 bytes. Anything else means the FujiNet
       encoded a different version, and the caller's buffer is the wrong size. */
    if (len != FUJI_QRCODE_V1_SIZE)
        return false;

    return fuji_qrcode_output((char *) out, FUJI_QRCODE_V1_SIZE);
}
