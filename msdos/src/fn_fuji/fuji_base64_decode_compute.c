#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_base64_decode_compute(void)
{
    return int_f5(0x70,0xCB,0x00,0x00) == 'C';
}
