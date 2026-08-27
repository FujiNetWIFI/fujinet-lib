#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
    return int_f5_read(0x70,0xBE,output_mode,0x00,len,sizeof(unsigned long)) == 'C';
}
