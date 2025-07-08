#include "fujinet-fuji.h"
#include "call_sio.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct {
  uint16_t length;
  uint8_t data[MAX_APPKEY_LEN];
} FNAppKeyString;

#define SIO_NONE	0x00
#define SIO_READ	0x40
#define SIO_WRITE	0x80

#define SIO_ERR_NONE	1

// DCB struct with exact layout and no padding
typedef struct {
  uint8_t DDEVIC;   // $0300
  uint8_t DUNIT;    // $0301
  uint8_t DCOMND;   // $0302
  uint8_t DSTATS;   // $0303
  void *DBUF;       // $0304-$0305 (little-endian)
  uint8_t DTIM;     // $0306
  uint8_t DUNUSE;   // $0307
  uint16_t DBYT;    // $0308-$0309
  uint8_t DAUX1;    // $030A
  uint8_t DAUX2;    // $030B
} SIO_DCB;

#define atari_dcb ((SIO_DCB*) 0x0300)

FNAppKeyString appkey_buf;

bool fuji_bus_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  // These arguments are for compatibility with other systems and not
  // used on Atari. Hack to suppress the warning that they are unused.
  (void) aux3;
  (void) aux4;
  (void) fields;

  atari_dcb->DDEVIC = device;
  atari_dcb->DUNIT = 1;
  atari_dcb->DCOMND = fuji_cmd;
  atari_dcb->DTIM = 0x0f;
  atari_dcb->DUNUSE = 0;
  atari_dcb->DAUX1 = aux1;
  atari_dcb->DAUX2 = aux2;
  atari_dcb->DSTATS = SIO_NONE;
  atari_dcb->DBUF = NULL;
  atari_dcb->DBYT = 0;

  if (reply) {
    atari_dcb->DSTATS = SIO_READ;
    atari_dcb->DBUF = reply;
    atari_dcb->DBYT = reply_length;
  }
  else if (data) {
    atari_dcb->DSTATS = SIO_WRITE;
    atari_dcb->DBUF = (void *) data;
    atari_dcb->DBYT = data_length;
  }

  return call_sio();
}

bool fuji_error(void)
{
  return atari_dcb->DSTATS != SIO_ERR_NONE;
}

/*
  appkeys are variable length strings. Because Atari SIO requires
  fixed length packets, an extra header must be added to indicate the
  true size of the key. On write the aux1/aux2 fields are combined
  into a uint16_t. On read, there is an extra 2 bytes of header to
  indicate how much of the fixed block represents the string.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
 // Caller may not have room for length header so use our own buffer to read
  if (!FUJICALL_RV(FUJICMD_READ_APPKEY, &appkey_buf, sizeof(appkey_buf)))
    return false;
  *length = appkey_buf.length;
  memmove(string, appkey_buf.data, appkey_buf.length);
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_B12_D(FUJICMD_WRITE_APPKEY, length, string, MAX_APPKEY_LEN);
}
