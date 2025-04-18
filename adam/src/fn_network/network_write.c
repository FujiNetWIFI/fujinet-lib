#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
{
    struct _w
    {
        uint8_t cmd;
        uint16_t len;
        uint8_t buf[1024];
    } w;

    uint8_t u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_NO_DEVICE;
    
    w.cmd = 'W';
    
    while (len)
    {
        w.len = (len > 1024) ? 1024 : len;
        memcpy(w.buf,buf,w.len);
        len -= w.len;

        if (eos_write_character_device(u,w,w.len + 3) != ADAMNET_OK)
            return FN_ERR_IO_ERROR;
    }

    return FN_ERR_OK;
}
