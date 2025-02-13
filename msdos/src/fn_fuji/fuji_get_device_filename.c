#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_get_device_filename(uint8_t ds, char *buffer)
{
    return int_f5_read(0x70,0xDA,ds,0x00,buffer,256) == 'C';
}
