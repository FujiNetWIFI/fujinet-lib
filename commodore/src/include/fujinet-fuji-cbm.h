#ifndef FUJINET_FUJI_CBM_H
#define FUJINET_FUJI_CBM_H

#include <stdbool.h>
#include <stdint.h>

#include "fujinet-cbm.h"

// track if we should open or write our commands to FN
extern bool fuji_is_open;

// common code for checking if we're a continuation and should use write, or a fresh open
bool open_or_write(uint8_t cmd);

// helpers for performing the boiler plate open/write/read/status sequences, depending on if there was any data to send etc.
bool open_close(uint8_t cmd);
bool open_read_close(uint8_t cmd, bool should_close, int *bytes_read, uint16_t result_size, uint8_t *result_data);

bool open_close_data(uint8_t cmd, bool should_close, uint16_t params_size, uint8_t *cmd_params);
// optimized versions of the above by direct parameter passing
bool open_close_data_1(uint8_t cmd, uint8_t param1);
bool open_close_data_2(uint8_t cmd, uint8_t param1, uint8_t param2);

bool open_read_close_data(uint8_t cmd, bool should_close, int *bytes_read, uint16_t params_size, uint8_t *cmd_params, uint16_t result_size, uint8_t *result_data);
// optimized versions of the above by direct parameter passing
bool open_read_close_data_1(uint8_t cmd, int *bytes_read, uint8_t param1, uint16_t result_size, uint8_t *result_data);
bool open_read_close_data_2(uint8_t cmd, int *bytes_read, uint8_t param1, uint8_t param2, uint16_t result_size, uint8_t *result_data);

// iec status values
extern FNStatus _fuji_status;

void status_error(uint8_t err_code, uint8_t cmd);
bool get_fuji_status(bool should_close);

#define ERROR_GLOBAL                 1
#define ERROR_STATUS_FAILED_TO_WRITE 2
#define ERROR_MALLOC_FAILED          3
#define ERROR_OPEN_CMD_FAILED        4
#define ERROR_OPEN_WRITE_CMD_FAILED  5

#endif // FUJINET_FUJI_CBM_H