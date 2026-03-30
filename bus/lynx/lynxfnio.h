/**
 *  for Atari Lynx
 */

#ifndef LYNXFNIO_H
#define LYNXFNIO_H

#include "fujinet-int.h"

// Max message size
#define FNIO_TX_LEN_MAX       1024
#define SERIAL_PACKET_SIZE    256

#define LYNX_TIMEOUT          10

enum FNIO_ERROR_T {
        FNIO_ERR_NONE,       // success
        FNIO_ERR_TIMEOUT,    // timed out waiting for data
        FNIO_ERR_SEND_CHK,   // checksum on our sent data was bad
        FNIO_ERR_RECV_CHK,   // checksum on received data was bad
        FNIO_ERR_NO_DATA,    // no data received
        FNIO_ERR_CMD_FAILED, // command failed on Fujinet side
        FNIO_ERR_GENERAL     // undefined error
};

// Some globals to help with code size/speed (could be moved to zero page)
#ifdef UNUSED
extern unsigned char _ck;    // checksum byte
extern char _r;              // response/data from FN
#endif /* UNUSED */
extern unsigned char _fn_error;


// helper functions
uint8_t _checksum(char *b, unsigned short len);
int _serial_get_loop(void);

// main functions
unsigned char fnio_error();
unsigned char fnio_init(void);
unsigned char fnio_done(void);
unsigned char fnio_send_buf(unsigned char dev, char *buf, unsigned int len);
bool fnio_recv_buf(char *buf, unsigned int *len, unsigned int maxlen);
unsigned char fnio_recv_ack(void);
void fnio_flush_recv(void);

#endif /* FUJINET_H */
