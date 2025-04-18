#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-network.h"
#include "fujinet-network-adam.h"

uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
{
    struct _ns
    {
        unsigned short bw;
        bool connected;
        unsigned char err;
    } ns;

    uint8_t u = network_unit_adamnet(devicespec);

    if (!u)
        return FN_ERR_NO_DEVICE;

    if (eos_write_character_device(u,"S",1) != ADAMNET_OK)
        return FN_ERR_IO_ERROR;

    if (eos_read_character_device(u,response,sizeof(response) != ADAMNET_OK))
        return FN_ERR_IO_ERROR;

    memcpy(ns,response,sizeof(response));

    if (bw)
        *bw = ns.bw;

    if (c)
        *c = ns.c;

    if (err)
        *err = ns.err;
    
    return FN_ERR_OK;
}
