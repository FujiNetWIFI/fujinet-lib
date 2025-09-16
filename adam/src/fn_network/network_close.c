#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"

uint8_t network_close(const char* devicespec)
{
    uint8_t err = 0;
    unsigned char u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_NO_DEVICE;
    
    while (1)
    {
        err = eos_write_character_device(u,"C",1);

        if (err == ADAMNET_TIMEOUT)
            continue;
        else if (err == ADAMNET_OK)
            return FN_ERR_OK;
        else
            return FN_ERR_IO_ERROR;
    }
}
