#include <stdint.h>
#include "fujinet-network.h"

uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode) {
    return network_ioctl('M', 0, mode, devicespec, 0, 0, 0);
}
