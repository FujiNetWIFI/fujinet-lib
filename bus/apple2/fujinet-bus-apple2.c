#include "fujinet-bus-apple2.h"
#include "fujinet-commands.h"
#include "fujinet-network.h"
#include "fujinet-err.h"
#include <string.h>
#include <stdarg.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX_SMARTPORT_BLOCK 512

typedef struct {
  uint16_t length;
  uint8_t data[];
} fujibus_packet;

static fujibus_packet *fb_packet = (fujibus_packet *) sp_payload;
bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields, ...)
{
  bool is_status;
  uint8_t sp_id;
  uint16_t idx, numbytes;
  va_list ap;


  if (device == FUJI_DEVICEID_FUJINET)
    sp_id = sp_get_fuji_id();
  else if (device == FUJI_DEVICEID_CLOCK)
    sp_id = sp_get_clock_id();
  else if (device == FUJI_DEVICEID_CPM)
    sp_id = sp_get_cpm_id();
#ifdef MIDI_SUPPORTED
  else if (device == FUJI_DEVICEID_MIDI)
    sp_id = sp_get_midi_id();
#endif /* MIDI_SUPPORTED */
  else if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    sp_id = sp_get_network_id();
    sp_nw_unit = device - FUJI_DEVICEID_NETWORK + 1;
  }
  else if (device >= FUJI_DEVICEID_PRINTER && device <= FUJI_DEVICEID_PRINTER_LAST)
    sp_id = sp_get_printer_id();
  else if (device >= FUJI_DEVICEID_SERIAL && device <= FUJI_DEVICEID_SERIAL_LAST)
    sp_id = sp_get_modem_id();
#ifdef VOICE_SUPPORTED
  else if (device == FUJI_DEVICEID_VOICE)
    sp_id = sp_get_voice_id();
#endif /* VOICE_SUPPORTED */
  else
    return false;

  if (sp_id == 0)
    return false;

  is_status = fields & FUJI_FIELD_REPLY;

  va_start(ap, fields);

  if (fields || !is_status) {
    idx = 0;
    numbytes = fuji_field_numbytes(fields);
    if (numbytes > 0)
      fb_packet->data[idx++] = va_arg(ap, uint8_t);
    if (numbytes > 1)
      fb_packet->data[idx++] = va_arg(ap, uint8_t);
    if (numbytes > 2)
      fb_packet->data[idx++] = va_arg(ap, uint8_t);
    if (numbytes > 3)
      fb_packet->data[idx++] = va_arg(ap, uint8_t);
    if (fields & FUJI_FIELD_DATA) {
      const uint8_t *data = va_arg(ap, uint8_t *);
      const uint16_t data_length = va_arg(ap, uint16_t);


      memcpy(&fb_packet->data[idx], data, data_length);
      idx += data_length;
    }

    fb_packet->length = idx;

    if (sp_id == sp_network)
      sp_control_nw(sp_id, fuji_cmd);
    else
      sp_control(sp_id, fuji_cmd);
  }

  if (is_status) {
    if (sp_id == sp_network)
      sp_status_nw(sp_id, fuji_cmd);
    else
      sp_status(sp_id, fuji_cmd);
    if (!sp_error && (fields & FUJI_FIELD_REPLY)) {
      uint8_t *reply = va_arg(ap, uint8_t *);
      uint16_t reply_length = va_arg(ap, uint16_t);


      if (sp_count < reply_length)
        reply_length = sp_count;
      memcpy(reply, &sp_payload[0], reply_length);
    }
  }

  va_end(ap);

  return !sp_error;
}

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  uint16_t err;
  uint8_t sp_id = sp_fuji_id;


  if (!length)
    return 0;

  if (sp_get_fuji_id() == 0)
    return 0;

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    sp_id = sp_network;
    sp_nw_unit = device - FUJI_DEVICEID_NETWORK + 1;
  }

  length = MIN(length, MAX_SMARTPORT_BLOCK);

  if (sp_id == sp_network)
    err = sp_read_nw(sp_id, length);
  else
    err = sp_read(sp_id, length);

  if (err)
    return 0;

  length = sp_count;
  memcpy(buffer, &sp_payload[0], length);
  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  uint16_t err;
  uint8_t sp_id = sp_fuji_id;


  if (!length)
    return 0;

  if (sp_get_fuji_id() == 0)
    return 0;

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    sp_id = sp_network;
    sp_nw_unit = device - FUJI_DEVICEID_NETWORK + 1;
  }

  length = MIN(length, MAX_SMARTPORT_BLOCK);
  memcpy(&sp_payload[0], buffer, length);

  if (sp_id == sp_network)
    err = sp_write_nw(sp_id, length);
  else
    err = sp_write(sp_id, length);

  if (err)
    return 0;

  return length;
}

/*
  appkeys are variable length strings. Because SmartPort supports
  variable length data packets, aux1/aux2 aren't used to send the
  length of the string. Instead only the string data is sent with no
  length field, no block size, no padding.
*/

bool fuji_bus_appkey_read(void *string, uint16_t *length)
{
  sp_status(sp_fuji_id, FUJICMD_READ_APPKEY);
  if (sp_error)
    return false;
  memcpy(string, &sp_payload[0], sp_count);
  *length = sp_count;
  return true;
}

bool fuji_bus_appkey_write(void *string, uint16_t length)
{
  return FUJICALL_D(FUJICMD_WRITE_APPKEY, string, length);
}
