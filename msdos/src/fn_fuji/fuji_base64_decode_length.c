#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_base64_decode_length(unsigned long *len)
{
    return int_f5_ah_40(0x70,0xCA,0x00,0x00,len,sizeof(unsigned long)) == 'C';
}
