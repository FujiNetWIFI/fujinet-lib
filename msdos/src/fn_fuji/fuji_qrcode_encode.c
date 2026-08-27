#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

// int_f5 carries only aux1 and aux2, so there is nowhere to put `shorten` and
// the FujiNet reads it as zero on this bus. Same limitation as Atari SIO.
bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
    (void) shorten;
    return int_f5(0x70, FUJICMD_QRCODE_ENCODE, version, ecc) == 'C';
}
