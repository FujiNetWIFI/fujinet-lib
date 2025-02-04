#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
{
    return int_f5_ah_40(0x70,0xC5,output_type,0x00,s,len) == 'C';
}
