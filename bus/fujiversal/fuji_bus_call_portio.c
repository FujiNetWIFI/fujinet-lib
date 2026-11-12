#include <fujinet-platform.h>

#ifdef FUJI_BUS_CALL_PORTIO

#include <stdio.h> // debug

#include "fujiversal.h"
#include "portio.h"
#include <fujinet-bus.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static bool port_did_init = false;

static uint16_t fuji_calc_checksum(const void *ptr, uint16_t len, uint16_t seed)
{
  uint16_t idx, chk;
  uint8_t *buf = (uint8_t *) ptr;


  for (idx = 0, chk = seed; idx < len; idx++)
    chk = ((chk + buf[idx]) >> 8) + ((chk + buf[idx]) & 0xFF);
  return chk;
}

static uint8_t fuji_packet_call(AtariSIODirection direction, fujibus_packet *packet_ptr,
                                void *pbuf, uint16_t plen)
{
  uint8_t saved_slot, my_slot;
  uint8_t ck1, ck2;
  uint16_t rlen;
  bool success = false;
  uint8_t pdev = packet_ptr->header.device;
  uint8_t aux_len = fuji_field_numbytes(packet_ptr->header.fields);


  if (!port_did_init) {
    PORT_INIT();
    port_did_init = true;
  }

  packet_ptr->header.length = sizeof(packet_ptr->header) + aux_len;
  if (direction == SIO_DIRECTION_WRITE)
    packet_ptr->header.length += plen;

  // Data is spread across two buffers: packet_ptr and pbuf
  ck1 = fuji_calc_checksum(packet_ptr, aux_len + sizeof(packet_ptr->header), 0);
  if (direction == SIO_DIRECTION_WRITE)
    ck1 = fuji_calc_checksum(pbuf, plen, ck1);
  packet_ptr->header.checksum = ck1;
#ifdef DEBUG
  hexdump(packet_ptr, sizeof(fujibus_header));
#endif /* DEBUG */

  // Whatever is waiting now is the tail of a reply that arrived after its
  // call gave up. Left there, it would be read as this call's reply.
  port_drain();

  port_putc(SLIP_END);
  port_putbuf_slip(packet_ptr, aux_len + sizeof(packet_ptr->header));
  if (direction == SIO_DIRECTION_WRITE)
    port_putbuf_slip(pbuf, plen);
  port_putc(SLIP_END);

  if (direction != SIO_DIRECTION_READ) {
    pbuf = NULL;
    plen = 0;
  }
  rlen = port_getbuf_slip_dual(packet_ptr, sizeof(packet_ptr->header),
                               pbuf, plen, TIMEOUT_SLOW);
  if (rlen < sizeof(fujibus_header) || rlen != packet_ptr->header.length) {
#ifdef DEBUG
    printf("Reply length incorrect: %d %d\n", rlen, packet_ptr->header.length);
    hexdump((uint8_t *) packet_ptr, sizeof(fujibus_header));
    hexdump(pbuf, rlen);
#endif /* DEBUG */
    success = false;
    goto done;
  }
#ifdef DEBUG
  if (rlen - sizeof(fujibus_header) != plen) {
    printf("Expected length incorrect: %d %d\n", rlen - sizeof(fujibus_header), plen);
    hexdump((uint8_t *) packet_ptr, sizeof(fujibus_header));
  }
#endif /* DEBUG */

  // Need to zero out checksum in order to calculate
  ck1 = packet_ptr->header.checksum;
  packet_ptr->header.checksum = 0;

  // Data is spread across two buffers: packet_ptr and pbuf
  ck2 = fuji_calc_checksum(packet_ptr, sizeof(packet_ptr->header), 0);
#ifdef DEBUG
  printf("CK2_A = 0x%02x\n", ck2);
#endif /* DEBUG */
  if (direction == SIO_DIRECTION_READ)
    ck2 = fuji_calc_checksum(pbuf, rlen - sizeof(packet_ptr->header), ck2);
#ifdef DEBUG
  printf("CK2_B = 0x%02x\n", ck2);
#endif /* DEBUG */

  if (ck1 != ck2) {
#ifdef DEBUG
    printf("Checksum mismatch: 0x%02x 0x%02x rlen=%d dir=0x%02x\n", ck1, ck2, rlen, direction);
    hexdump(packet_ptr, sizeof(fujibus_header));
    exit(1);
#endif /* DEBUG */
    success = false;
    goto done;
  }

  if (packet_ptr->header.device != pdev) {
#ifdef DEBUG
    printf("Incorrect device: R:0x%02x E:0x%02x\n", packet_ptr->header.device, pdev);
    hexdump((uint8_t *) packet_ptr, sizeof(packet_ptr->header));
#endif /* DEBUG */
    success = false;
    goto done;
  }

  if (packet_ptr->header.command != PACKET_ACK) {
#ifdef DEBUG
    printf("Not ACK: 0x%02x\n", packet_ptr->header.command);
#endif /* DEBUG */
    success = false;
    goto done;
  }

  // FIXME - validate that packet_ptr->fields is zero?

  success = true;

 done:
  return success;
}

bool fuji_bus_call_portio(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                          uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                          const void *buf, size_t buf_length)
{
  int code;
  uint8_t ck1, ck2;
  uint16_t rlen;
  uint16_t idx, numbytes;
  fujibus_packet fb_packet;
  AtariSIODirection direction;


#ifdef UNUSED
  if (device != FUJI_DEVICEID_FUJINET) {
    printf("Device  = 0x%02x\n", device);
    printf("Command = 0x%02x\n", fuji_cmd);
    printf("Fields  = 0x%02x\n", fields);
    printf("AUX     = 0x%02x 0x%02x 0x%02x 0x%02x\n", aux1, aux2, aux3, aux4);
    printf("Buf len = %d\n", buf_length);
    exit(1);
  }
#endif /* UNUSED */

  fb_packet.header.device = device;
  fb_packet.header.command = fuji_cmd;
  fb_packet.header.length = sizeof(fujibus_header);
  fb_packet.header.checksum = 0;
  fb_packet.header.fields = fields;

  idx = 0;
  numbytes = fuji_field_numbytes(fields);
  if (numbytes > 0)
    fb_packet.data[idx++] = aux1;
  if (numbytes > 1)
    fb_packet.data[idx++] = aux2;
  if (numbytes > 2)
    fb_packet.data[idx++] = aux3;
  if (numbytes > 3)
    fb_packet.data[idx++] = aux4;

  direction = fields & FUJI_FIELD_REPLY ? SIO_DIRECTION_READ : SIO_DIRECTION_WRITE;
  return fuji_packet_call(direction, &fb_packet, buf, buf_length);
}

#endif /* FUJI_BUS_CALL_PORTIO */
