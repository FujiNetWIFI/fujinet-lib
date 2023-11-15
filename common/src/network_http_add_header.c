#include <stdint.h>
#include <string.h>
#include "fujinet-network.h"

uint8_t network_http_add_header(char *devicespec, char *header) {
    return network_write(devicespec, (uint8_t *) header, strlen(header));
}
