#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_put_host_slots(HostSlot *h, size_t size)
{
    return int_f5_write(0x70,0xF3,0x00,0x00,h,size) == 'C';
}
