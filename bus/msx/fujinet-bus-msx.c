#include "fujinet-bus-msx.h"
#include "portio.h"

#define TIMEOUT         100
#define TIMEOUT_SLOW	15 * 1000
#define MAX_RETRIES	5

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

uint8_t fujicom_cksum(void *ptr, uint16_t len)
{
  uint16_t chk = 0;
  int i = 0;
  uint8_t *buf = (uint8_t *) ptr;


  for (i = 0; i < len; i++)
    chk = ((chk + buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);
  return (uint8_t) chk;
}

bool fuji_bus_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint8_t reply, retries;
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
  port_putbuf(&fb_packet, sizeof(fb_packet));

  for (retries = 0; retries < MAX_RETRIES; retries++) {
    reply = port_getc_timeout(TIMEOUT);
    if (reply == PACKET_NAK)
      return false;

    if (reply == PACKET_ACK)
      break;
  }

  if (retries == MAX_RETRIES)
    return false;

  if (reply) {
    reply = port_getc_timeout(TIMEOUT_SLOW);
    if (reply != PACKET_COMPLETE)
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
  else if (data) {
    /* Write the payload */
    port_putbuf(reply, reply_length);

    /* Write the checksum */
    ck1 = fujicom_cksum(reply, reply_length);
    port_putc(ck1);

    /* Wait for ACK/NACK */
    reply = port_getc_timeout(TIMEOUT_SLOW);
    if (reply != PACKET_ACK)
      return false;

    /* Wait for COMPLETE/ERROR */
    reply = port_getc_timeout(TIMEOUT_SLOW);
    if (reply != PACKET_COMPLETE)
      return false;
  }

  return true;
}

