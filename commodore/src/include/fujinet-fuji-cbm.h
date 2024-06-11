#ifndef FUJINET_FUJI_CBM_H
#define FUJINET_FUJI_CBM_H

#include <stdbool.h>
#include <stdint.h>

// Our custom cbm_open that works with binary data by not doing strlen on the name, but instead takes a length parameter
uint8_t fuji_cbm_open(uint8_t lfn, uint8_t device, uint8_t sec_addr, uint8_t len, uint8_t* name);

// helpers for performing the boiler plate open/write/read/status sequences, depending on if there was any data to send etc.
bool open_close(uint8_t cmd);
bool open_read_close(uint8_t cmd, int *bytes_read, uint16_t result_size, uint8_t *result_data);

bool open_close_data(uint8_t cmd, uint16_t params_size, uint8_t *cmd_params);
// optimized versions of the above by direct parameter passing
bool open_close_data_1(uint8_t cmd, uint8_t param1);
bool open_close_data_2(uint8_t cmd, uint8_t param1, uint8_t param2);

bool open_read_close_data(uint8_t cmd, int *bytes_read, uint16_t params_size, uint8_t *cmd_params, uint16_t result_size, uint8_t *result_data);
// optimized versions of the above by direct parameter passing
bool open_read_close_data_1(uint8_t cmd, int *bytes_read, uint8_t param1, uint16_t result_size, uint8_t *result_data);
bool open_read_close_data_2(uint8_t cmd, int *bytes_read, uint8_t param1, uint8_t param2, uint16_t result_size, uint8_t *result_data);

// iec status values
extern uint8_t iec_error;
extern uint8_t iec_channel;
extern bool iec_connected;
extern char *iec_message;

void status_error();
bool get_fuji_status();

#endif // FUJINET_FUJI_CBM_H