#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
{
    unsigned char aux2 = mode & 0x0F | (hs << 4);

    return int_f5_ah_80(0x70,0xE2,ds,aux2,buffer,256) == 'C';
}
