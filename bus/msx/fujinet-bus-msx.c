#undef UNUSED
#ifdef UNUSED
#include <stdio.h>
#endif /* UNUSED */

#include "fujinet-bus-msx.h"
#include "fujinet-commands.h"
#include "portio.h"
#include <string.h>

#ifdef UNUSED
#define COLUMNS 16

static void hexdump(uint8_t *buffer, int count)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < count; outer += COLUMNS) {
    for (inner = 0; inner < COLUMNS; inner++) {
      if (inner + outer < count) {
	c = buffer[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < COLUMNS && inner + outer < count; inner++) {
      c = buffer[inner + outer];
      if (c >= ' ' && c <= 0x7f)
	printf("%c", c);
      else
	printf(".");
    }
    printf("|\n");
  }

  return;
}
#endif /* UNUSED */


#define milliseconds_to_jiffy(millis) ((millis) / (VDP_IS_PAL ? 20 : 1000 / 60))

#define TIMEOUT         milliseconds_to_jiffy(100)
#define TIMEOUT_SLOW	milliseconds_to_jiffy(15 * 1000)
#define MAX_RETRIES	1

enum {
  SLIP_END     = 0xC0,
  SLIP_ESCAPE  = 0xDB,
  SLIP_ESC_END = 0xDC,
  SLIP_ESC_ESC = 0xDD,
};

enum {
  PACKET_ACK = 6, // ASCII ACK
  PACKET_NAK = 21, // ASCII NAK
};

typedef struct {
  uint8_t device;   /* Destination Device */
  uint8_t command;  /* Command */
  uint16_t length;  /* Total length of packet including header */
  uint8_t checksum; /* Checksum of entire packet */
  uint8_t fields;   /* Describes the fields that follow */
} fujibus_header;

typedef struct {
  fujibus_header header;
  uint8_t data[];
} fujibus_packet;

#define MAX_PACKET      (512 + sizeof(fujibus_header) + 4) // sector + header + secnum
static uint8_t fb_buffer[MAX_PACKET * 2 + 2];              // Enough room for SLIP encoding
static fujibus_packet *fb_packet;

/* This function expects that fb_packet is one byte into fb_buffer so
   that there's already room at the front for the SLIP_END framing
   byte. This allows skipping moving all the bytes if no escaping is
   needed. */
uint16_t fuji_slip_encode()
{
  uint16_t idx, len, enc_idx, esc_count, esc_remain;
  uint8_t ch, *ptr;


  // Count how many bytes need to be escaped
  len = fb_packet->header.length;
  ptr = (uint8_t *) fb_packet;
  for (idx = esc_count = 0; idx < len; idx++) {
    if (ptr[idx] == SLIP_END || ptr[idx] == SLIP_ESCAPE)
      esc_count++;
  }

#ifdef UNUSED
  printf("ESC count: %d %d\n", esc_count, len);
#endif /* UNUSED */
  if (esc_count) {
    // Encode buffer in place working from back to front
    for (esc_remain = esc_count, enc_idx = esc_count + (idx = len - 1);
         esc_remain;
         idx--, enc_idx--) {
      ch = ptr[idx];
      if (ch == SLIP_END) {
        ptr[enc_idx--] = SLIP_ESC_END;
        ch = SLIP_ESCAPE;
        esc_remain--;
      }
      else if (ch == SLIP_ESCAPE) {
        ptr[enc_idx--] = SLIP_ESC_ESC;
        ch = SLIP_ESCAPE;
        esc_remain--;
      }

      ptr[enc_idx] = ch;
    }
  }

  // FIXME - this byte probably never changes, maybe we should init fb_buffer with it?
  fb_buffer[0] = SLIP_END;
  fb_buffer[1 + len + esc_count] = SLIP_END;
  return 2 + len + esc_count;
}

uint16_t fuji_slip_decode(uint16_t len)
{
  uint16_t idx, dec_idx, esc_count;
  uint8_t *ptr;


  ptr = (uint8_t *) fb_packet;
  for (idx = dec_idx = 0; idx < len; idx++, dec_idx++) {
    if (ptr[idx] == SLIP_END)
      break;

    if (ptr[idx] == SLIP_ESCAPE) {
      idx++;
      if (ptr[idx] == SLIP_ESC_END)
        ptr[dec_idx] = SLIP_END;
      else if (ptr[idx] == SLIP_ESC_ESC)
        ptr[dec_idx] = SLIP_ESCAPE;
    }
    else if (idx != dec_idx) {
      // Only need to move byte if there were escapes decoded
      ptr[dec_idx] = ptr[idx];
    }
  }

  return dec_idx;
}

uint8_t fuji_calc_checksum(void *ptr, uint16_t len)
{
  uint16_t idx, chk;
  uint8_t *buf = (uint8_t *) ptr;


  for (idx = chk = 0; idx < len; idx++)
    chk = ((chk + buf[idx]) >> 8) + ((chk + buf[idx]) & 0xFF);
  return (uint8_t) chk;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  int code;
  uint8_t ck1, ck2;
  uint16_t rlen;
  uint16_t idx, numbytes;


  fb_packet = (fujibus_packet *) (fb_buffer + 1); // +1 for SLIP_END
#ifdef UNUSED
  printf("buf: 0x%04x pak: 0x%04x\n", fb_buffer, fb_packet);
  printf("fields: %d\n", fields);
#endif /* UNUSED */
  fb_packet->header.device = device;
  fb_packet->header.command = fuji_cmd;
  fb_packet->header.length = sizeof(fujibus_header);
  fb_packet->header.checksum = 0;
  fb_packet->header.fields = fields;
#ifdef UNUSED
  printf("Header len %d %d\n", fb_packet->header.length, sizeof(fujibus_header));
  hexdump((uint8_t *) fb_packet, sizeof(fujibus_header));
#endif /* UNUSED */

  idx = 0;
  numbytes = fuji_field_numbytes(fields);
#ifdef UNUSED
  printf("numbytes: %d %d\n", fields, numbytes);
  hexdump(fuji_field_numbytes_table, 8);
#endif /* UNUSED */
  if (numbytes) {
    fb_packet->data[idx++] = aux1;
    numbytes--;
  }
  if (numbytes) {
    fb_packet->data[idx++] = aux2;
    numbytes--;
  }
  if (numbytes) {
    fb_packet->data[idx++] = aux3;
    numbytes--;
  }
  if (numbytes) {
    fb_packet->data[idx++] = aux4;
    numbytes--;
  }
  if (data) {
    memcpy(&fb_packet->data[idx], data, data_length);
    idx += data_length;
  }
#ifdef UNUSED
  printf("Fields + data %d %d\n", numbytes, idx);
#endif /* UNUSED */

  fb_packet->header.length += idx;
#ifdef UNUSED
  printf("Packet len %d\n", fb_packet->header.length);
#endif /* UNUSED */

  ck1 = fuji_calc_checksum(fb_packet, fb_packet->header.length);
#ifdef UNUSED
  printf("Checksum: 0x%02x\n", ck1);
  hexdump((uint8_t *) fb_packet, sizeof(fujibus_header));
#endif /* UNUSED */
  fb_packet->header.checksum = ck1;

  numbytes = fuji_slip_encode();

#ifdef UNUSED
  printf("Sending packet %d\n", numbytes);
  //hexdump(fb_buffer, numbytes);
#endif /* UNUSED */
  port_putbuf(fb_buffer, numbytes);
#if 0
  code = port_discard_until(SLIP_END, TIMEOUT_SLOW);
#else
  while (1) {
    code = port_getc_timeout(TIMEOUT_SLOW);
#ifdef UNUSED
    printf("%02x ", code);
#endif /* UNUSED */
    if (code < 0 || code == SLIP_END)
      break;
  }
#endif // 0
  if (code != SLIP_END) {
#ifdef UNUSED
    printf("NO SLIP FRAME %d\n", code);
#endif /* UNUSED */
    return false;
  }

  rlen = port_get_until(fb_packet, (fb_buffer + sizeof(fb_buffer)) - ((uint8_t *) fb_packet),
                 SLIP_END, TIMEOUT_SLOW);
#ifdef UNUSED
  printf("Packet reply: %d\n", rlen);
  hexdump((uint8_t *) fb_packet, sizeof(fujibus_header));
#endif /* UNUSED */
  rlen = fuji_slip_decode(rlen);
#ifdef UNUSED
  printf("Decode len: %d %d\n", rlen, fb_packet->header.length);
  hexdump((uint8_t *) fb_packet, sizeof(fujibus_header));
#endif /* UNUSED */
  if (rlen != fb_packet->header.length) {
#ifdef UNUSED
    printf("Reply length incorrect: %d %d\n", rlen, fb_packet->header.length);
#endif /* UNUSED */
    return false;
  }
#ifdef UNUSED
  if (rlen - sizeof(fujibus_header) != reply_length) {
    printf("Expected length incorrect: %d %d\n", rlen - sizeof(fujibus_header), reply_length);
  }
#endif /* UNUSED */

  // Need to zero out checksum in order to calculate
  ck1 = fb_packet->header.checksum;
  fb_packet->header.checksum = 0;
  ck2 = fuji_calc_checksum(fb_packet, rlen);
  if (ck1 != ck2) {
#ifdef UNUSED
    printf("Checksum mismatch: %02x %02x\n", ck1, ck2);
#endif /* UNUSED */
    return false;
  }

  // FIXME - validate that fb_packet->fields is zero?

  if (reply_length && rlen) {
    if (reply_length < rlen)
      rlen = reply_length;
    memcpy(reply, fb_packet->data, rlen);
  }

  return true;
}

uint16_t fuji_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

uint16_t fuji_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}
