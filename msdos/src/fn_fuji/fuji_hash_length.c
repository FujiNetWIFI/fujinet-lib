#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_hash_length(uint8_t mode)
{
    uint8_t len=0;

    if (int_f5_read(0x70,0xC6,mode,0x00,(void *)&len,sizeof(uint8_t)) == 'C')
        return len;

    return 0;
}
