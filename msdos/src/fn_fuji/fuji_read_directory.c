#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
    return int_f5_ah_40(0x70,0xF6,maxlen,aux2,buffer,maxlen) == 'C';
}
