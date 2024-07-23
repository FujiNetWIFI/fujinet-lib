#ifndef FUJINET_NETWORK_CBM_H
#define FUJINET_NETWORK_CBM_H

#include <cbm.h>
#include <stdbool.h>
#include <stdint.h>

#include "fujinet-cbm.h"

// extern bool network_is_open;
extern uint8_t network_num_channels_open;

uint8_t getDeviceNumber(const char* input, const char** afterDeviceSpec);

#define ERROR_GLOBAL                 1
#define ERROR_OPEN_CMD_FAILED        2

#endif // FUJINET_NETWORK_CBM_H