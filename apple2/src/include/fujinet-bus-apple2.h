
/**
 * @brief FujiNet Network Device Library Apple2 Base Functions
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_BUS_APPLE2_H
#define FUJINET_BUS_APPLE2_H

#include <stdint.h>

// These are for Apple2 SmartPort C headers for internal and not exposed in the normal fujinet header files

#define MAX_DATA_LEN (767)

extern uint8_t sp_is_init;

// device ids
extern int8_t sp_network;
// and then I started using _id
extern int8_t sp_clock_id;
extern int8_t sp_cpm_id;
extern int8_t sp_fuji_id;
extern int8_t sp_modem_id;
extern int8_t sp_printer_id;

// the general payload buffer
extern uint8_t sp_payload[];

// the dispatch function used for doing SP calls for a particular card
extern uint8_t sp_dispatch_fn[2];

// count of bytes the status request returned
extern uint16_t sp_count;

// global sp error value set by various routines
extern int8_t sp_error;

void sp_clr_payload();
int8_t sp_status(uint8_t dest, uint8_t statcode);
int8_t sp_control(uint8_t dest, uint8_t ctrlcode);
int8_t sp_read(uint8_t dest, uint16_t len);
int8_t sp_init();
int8_t sp_find_fuji();
int8_t sp_get_fuji_id();

#endif
