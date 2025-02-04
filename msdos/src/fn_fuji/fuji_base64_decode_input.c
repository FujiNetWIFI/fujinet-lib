#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_base64_decode_input(char *s, uint16_t len)
{
    return int_f5_ah_80(0x70,0xCC,len&0xFF,len>>8,s,len) == 'C';
}
