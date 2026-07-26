#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
    return int_f5(0x70,0xBD,version,(ecc&0x03)|(shorten?0x10:0x00)) == 'C';
}
