#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_generate_guid(char *buffer)
{
    return int_f5_read(0x70,0xBB,0x00,0x00,buffer,MAX_GUID_LEN) == 'C';
}
