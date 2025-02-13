#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_open_directory(uint8_t hs, char *path_filter)
{
    return int_f5_write(0x70,0xF7,hs,0x00,path_filter,256) == 'C';
}
