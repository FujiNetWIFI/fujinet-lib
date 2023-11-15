#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"

uint8_t network_http_post(char *devicespec, char *data) {
    uint8_t err;
    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
    if (err) return err;
    err = network_write(devicespec, (uint8_t *) data, strlen(data));
    if (err) return err;
    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}
