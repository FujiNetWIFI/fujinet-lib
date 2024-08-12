#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"

uint8_t network_close(char* devicespec)
{
    unsigned char u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_NO_DEVICE;
    
    eos_write_character_device(u,"C",1);

    return FN_ERR_OK;
}
