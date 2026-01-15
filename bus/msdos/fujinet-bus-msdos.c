#include "fujinet-bus-msdos.h"
#include "fujinet-commands.h"
#include "portio.h"

#define milliseconds_to_jiffy(millis) ((millis) / (VDP_IS_PAL ? 20 : 1000 / 60))

#define TIMEOUT         100
#define TIMEOUT_SLOW	(15 * 1000)
#define MAX_RETRIES	1

enum {
  PACKET_ACK = 'A',
  PACKET_NAK = 'N',
  PACKET_COMPLETE = 'C',
  PACKET_ERROR = 'E',
};

typedef union {         /* Command Frame */
  struct {
    union {
      struct {
        uint8_t device; /* Destination Device */
        uint8_t command;  /* Command */
      };
      uint16_t devcom;
    };
    union {
      struct {
        uint8_t aux1;   /* Auxiliary Parameter 1 */
        uint8_t aux2;   /* Auxiliary Parameter 2 */
        uint8_t aux3;   /* Auxiliary Parameter 3 */
        uint8_t aux4;   /* Auxiliary Parameter 4 */
      };
      struct {
        uint16_t aux12;
        uint16_t aux34;
      };
      uint32_t aux;
    };
    uint8_t cksum;               /* 8-bit checksum */
  };
} cmdFrame_t;

cmdFrame_t fb_packet;

uint8_t fujicom_cksum(const void *ptr, uint16_t len)
{
  uint16_t chk = 0;
  int i = 0;
  uint8_t *buf = (uint8_t *) ptr;


  for (i = 0; i < len; i++)
    chk = ((chk + buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);
  return (uint8_t) chk;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint8_t code, retries;
  uint8_t ck1, ck2;
  uint16_t rlen;


  fb_packet.device = device;
  fb_packet.command = fuji_cmd;

  fb_packet.aux1 = aux1;
  fb_packet.aux2 = aux2;
  fb_packet.aux3 = aux3;
  fb_packet.aux4 = aux4;

  fb_packet.cksum = fujicom_cksum(&fb_packet, sizeof(fb_packet) - sizeof(fb_packet.cksum));
  // FIXME - encode packet as SLIP

  for (retries = 0; retries < MAX_RETRIES; retries++) {
    // Flush out any data in RX buffer
    while (port_getc() >= 0)
      ;

    port_putbuf(&fb_packet, sizeof(fb_packet));
    code = port_getc_timeout(TIMEOUT);
    if (code == PACKET_NAK)
      return false;

    if (code == PACKET_ACK)
      break;
  }

  if (retries == MAX_RETRIES)
    return false;

  if (reply) {
    code = port_getc_timeout(TIMEOUT_SLOW);
    if (code != PACKET_COMPLETE)
      return false;

    /* Complete, get payload */
    rlen = port_getbuf(reply, reply_length, TIMEOUT);
    if (rlen != reply_length)
      return false;

    /* Get Checksum byte, verify it. */
    ck1 = port_getc_timeout(TIMEOUT_SLOW);
    ck2 = fujicom_cksum(reply, rlen);

    if (ck1 != ck2)
      return false;
  }
  else {
    if (data) {
      /* Write the payload */
      port_putbuf(data, data_length);

      /* Write the checksum */
      ck1 = fujicom_cksum(data, data_length);
      port_putc(ck1);

      /* Wait for ACK/NACK */
      code = port_getc_timeout(TIMEOUT_SLOW);
      if (code != PACKET_ACK)
        return false;
    }

    /* Wait for COMPLETE/ERROR */
    code = port_getc_timeout(TIMEOUT_SLOW);
    if (code != PACKET_COMPLETE)
      return false;
  }

  return true;
}

size_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

size_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}
