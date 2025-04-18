#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

/**
 * @brief Open network connection
 * @param devicespec Device spec to use "N:..."
 * @param mode connection mode
 * @param trans Translation mode 0 = none ...
 * @return FN_error code.
 */
uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
{
    struct _o
    {
        unsigned char cmd;
        unsigned char mode;
        unsigned char trans;
        char url[256];
    } o;

    uint8_t u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_BAD_CMD;
    
    o.cmd = 'O'; // open
    o.mode = mode;
    o.trans = trans;

    strncpy(o.url, devicespec, sizeof(o.url));

    if (eos_write_character_device(u, (unsigned char *)o, sizeof(o)) != ADAMNET_OK)
        return FN_ERR_IO_ERROR;
    else
        return FN_ERR_OK;
}
