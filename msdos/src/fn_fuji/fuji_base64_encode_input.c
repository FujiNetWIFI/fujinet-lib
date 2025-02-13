#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_base64_encode_input(char *s, uint16_t len)
{
    return int_f5_write(0x70,0xD0,len&0xFF,len>>8,s,len) == 'C';
}
