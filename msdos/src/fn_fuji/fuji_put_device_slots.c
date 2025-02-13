#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_put_device_slots(DeviceSlot *d, size_t size)
{
    return int_f5_write(0x70,0xF1,0x00,0x00,d,size) == 'C';
}
