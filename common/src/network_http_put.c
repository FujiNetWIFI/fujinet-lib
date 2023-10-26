#include <stdint.h>
#include "../../fujinet-network.h"

uint8_t network_http_put(char *devicespec, char *data) {
    return network_http_post(devicespec, data);
}
