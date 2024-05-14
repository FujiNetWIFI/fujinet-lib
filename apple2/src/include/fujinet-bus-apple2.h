
/**
 * @brief FujiNet Network Device Library Apple2 Base Functions
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_BUS_APPLE2_H
#define FUJINET_BUS_APPLE2_H

#include <stdint.h>

// These are for Apple2 SmartPort C headers for internal and not exposed in the normal fujinet header files

#define MAX_DATA_LEN (767)


#define SP_CMD_STATUS           (0)
#define SP_CMD_CONTROL          (4)
#define SP_CMD_OPEN             (6)
#define SP_CMD_CLOSE            (7)
#define SP_CMD_READ             (8)
#define SP_CMD_WRITE            (9)
#define SP_STATUS_PARAM_COUNT   (3)
#define SP_CONTROL_PARAM_COUNT  (3)
#define SP_OPEN_PARAM_COUNT     (1)
#define SP_CLOSE_PARAM_COUNT    (1)
#define SP_READ_PARAM_COUNT     (4)
#define SP_WRITE_PARAM_COUNT    (4)

#define SP_ERR_OK               (0x00)
#define SP_ERR_BAD_CMD          (0x01)
#define SP_ERR_BAD_PCNT         (0x02)
#define SP_ERR_BUS_ERR          (0x06)
#define SP_ERR_BAD_UNIT         (0x11)
#define SP_ERR_BAD_CTRL         (0x21)
#define SP_ERR_BAD_CTRL_PARM    (0x22)
#define SP_ERR_IO_ERROR         (0x27)
#define SP_ERR_NO_DRIVE         (0x28)
#define SP_ERR_NO_WRITE         (0x2B)
#define SP_ERR_BAD_BLOCK        (0x2D)
#define SP_ERR_DISK_SW          (0x2E)
#define SP_ERR_OFFLINE          (0x2F)
#define SP_ERR_DEV_SPEC0        (0x30)
#define SP_ERR_DEV_SPECF        (0x3F)
// SP_ERR_RESERVED $40-$4F
#define SP_ERR_NON_FATAL50      (0x50)
#define SP_ERR_NON_FATAL7F      (0x7F)

#define SP_PAYLOAD_SIZE         (512)


extern uint8_t sp_is_init;

// device ids
extern uint8_t sp_network;
// and then I started using _id
extern uint8_t sp_clock_id;
extern uint8_t sp_cpm_id;
extern uint8_t sp_fuji_id;
extern uint8_t sp_modem_id;
extern uint8_t sp_printer_id;

// the general payload buffer
extern uint8_t sp_payload[];

// cmd data that is communicated to the SP device
extern uint8_t sp_cmdlist[10];

// the dispatch function used for doing SP calls for a particular card
extern uint8_t sp_dispatch_fn[2];

// invoke smartport command
int8_t sp_dispatch(uint8_t cmd);

// count of bytes the status request returned
extern uint16_t sp_count;

// global sp error value set by various routines
extern int8_t sp_error;

void sp_clr_payload();
int8_t sp_status(uint8_t dest, uint8_t statcode);
int8_t sp_control(uint8_t dest, uint8_t ctrlcode);
int8_t sp_read(uint8_t dest, uint16_t len);
int8_t sp_open(uint8_t dest);

// initilises the dispatch funciton and returns the network device id if found, else returns 0.
uint8_t sp_init();

// These return the found device id or 0 if not found.
uint8_t sp_get_fuji_id();
uint8_t sp_get_network_id();
uint8_t sp_get_clock_id();
uint8_t sp_get_cpm_id();
uint8_t sp_get_modem_id();
uint8_t sp_get_printer_id();

int sp_find_device(char *name);

#endif
