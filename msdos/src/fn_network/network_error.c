#include <stdbool.h>
#include <stdint.h>
#include <fujinet-network.h>
#include <fujinet-fuji-msdos.h>
#include <string.h>

uint8_t network_error(const char *devicespec)
{
    uint8_t err = 0;
    uint8_t status_ret = 0;

    status_ret = network_status(devicespec, NULL, NULL, &err);

    return status_ret;
}
