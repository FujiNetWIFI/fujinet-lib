#include <stdint.h>
#include "fujinet-network.h"

uint8_t network_http_end_add_headers(char *devicespec) {
    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
}
