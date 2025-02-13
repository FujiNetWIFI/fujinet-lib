#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_enable_udpstream(uint16_t port, char *host)
{
    return int_f5_write(0x70,0xF0,port&0xFF,port>>8,host,64) == 'C';
}
