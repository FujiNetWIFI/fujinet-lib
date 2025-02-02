#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */
#include "fujinet-network.h"

uint8_t network_http_post_bin(const char *devicespec, const uint8_t *data, uint16_t len) {
    uint8_t err;
    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
    if (err) return err;
    err = network_write(devicespec, data, len);
    if (err) return err;
    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}
