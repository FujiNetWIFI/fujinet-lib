#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_base64_encode_output(char *s, uint16_t len)
{
    return int_f5_ah_40(0x70,0xCD,len&0xFF,len>>8,s,len) == 'C';
}
