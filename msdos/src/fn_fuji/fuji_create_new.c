#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_create_new(NewDisk *new_disk)
{
    // FIXME: come back here when we have a ready.
    return int_f5_write(0x70,0xE7,0x00,0x00,new_disk,sizeof(new_disk)) == 'C';
}
