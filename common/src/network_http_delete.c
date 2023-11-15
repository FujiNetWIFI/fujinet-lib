#include <stdint.h>
#include "fujinet-network.h"

uint8_t network_http_delete(char *devicespec, uint8_t trans) {
    return network_open(devicespec, OPEN_MODE_HTTP_DELETE_H, trans);
}
