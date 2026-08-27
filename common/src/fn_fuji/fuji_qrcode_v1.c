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
 * Within version 1 we then take the STRONGEST error correction the payload
 * leaves room for. Damage tolerance is free capacity that would otherwise go to
 * padding, and a low-ECC symbol on a CRT, photographed at an angle, is
 * noticeably harder to scan.
 */

/* Version 1 capacity in characters, indexed by qr_ecc_t (LOW..HIGH), for each
 * of the three encoding modes. Measured against libqrencode rather than copied
 * from a table. */
static const uint8_t qr_v1_capacity_num[4]   = { 41, 34, 27, 17 };
static const uint8_t qr_v1_capacity_alnum[4] = { 25, 20, 16, 10 };
static const uint8_t qr_v1_capacity_bytes[4] = { 17, 14, 11,  7 };

/* Which mode the encoder will choose for this string, as a capacity table.
 *
 * The order matters: digits are also in the alphanumeric set, so an all-digit
 * string has to be recognised as numeric first or it gets the smaller
 * alphanumeric capacity and a weaker error correction level than it could
 * carry. Anything outside the alphanumeric set forces byte mode, where a
 * version 1 symbol holds substantially less. */
static const uint8_t *qr_v1_capacity(const char *s, uint16_t n)
{
    uint16_t i;
    char c;
    bool numeric = true;

    for (i = 0; i < n; i++) {
        c = s[i];
        if (c >= '0' && c <= '9')
            continue;
        numeric = false;
        if (c >= 'A' && c <= 'Z')
            continue;
        if (c == ' ' || c == '$' || c == '%' || c == '*' ||
            c == '+' || c == '-' || c == '.' || c == '/' || c == ':')
            continue;
        return qr_v1_capacity_bytes;
    }

    return numeric ? qr_v1_capacity_num : qr_v1_capacity_alnum;
}

bool fuji_qrcode_v1(const char *s, uint8_t *out)
{
    const uint8_t *capacity;
    unsigned long len = 0;
    uint16_t n;
    uint8_t ecc;

    if (s == NULL || out == NULL)
        return false;

    n = (uint16_t) strlen(s);
    if (n == 0)
        return false;

    capacity = qr_v1_capacity(s, n);

    /* Reject locally rather than after three round trips to the FujiNet. Note
       the limit depends on the mode: 41 characters if the string is all digits,
       25 if it is entirely within the QR alphanumeric set, but only 17 if a
       single character is not -- so an all-uppercase url fits far more than a
       mixed-case one. */
    if (n > capacity[QR_ECC_LOW])
        return false;

    for (ecc = QR_ECC_HIGH; ecc > QR_ECC_LOW; ecc--) {
        if (n <= capacity[ecc])
            break;
    }

    if (!fuji_qrcode_input((char *) s, n))
        return false;

    if (!fuji_qrcode_encode(1, ecc, false))
        return false;

    if (!fuji_qrcode_length(QR_OUTPUT_BINARY, &len))
        return false;

    /* A version 1 symbol is always 57 bytes. Anything else means the FujiNet
       encoded a different version, and the caller's buffer is the wrong size. */
    if (len != FUJI_QRCODE_V1_SIZE)
        return false;

    return fuji_qrcode_output((char *) out, FUJI_QRCODE_V1_SIZE);
}
