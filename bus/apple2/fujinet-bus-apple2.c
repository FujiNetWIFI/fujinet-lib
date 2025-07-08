#include "fujinet-bus-apple2.h"
#include "fujinet-commands.h"
#include "fujinet-network.h"
#include "fujinet-err.h"
#include <string.h>

uint8_t fn_device_error;

#define MAX_SMARTPORT_BLOCK 512

#define FUJICMD_HIGHEST FUJICMD_RESET
#define FUJICMD_LOWEST  FUJICMD_RENAME

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  uint16_t length;
  uint8_t data[];
} fujibus_packet;

static fujibus_packet *fb_packet = (fujibus_packet *) sp_payload;
static uint8_t do_status[FUJICMD_HIGHEST - FUJICMD_LOWEST + 1];
static uint8_t did_status_init = 0;

#define CONFIG_STATUS(x) do_status[x - FUJICMD_LOWEST]
static void status_init()
{
  uint8_t idx;


  for (idx = 0; idx < 8; idx++)
    CONFIG_STATUS(FUJICMD_GET_DEVICE1_FULLPATH + idx) = 1;

  CONFIG_STATUS(FUJICMD_GET_ADAPTERCONFIG) = 1;
  CONFIG_STATUS(FUJICMD_GET_ADAPTERCONFIG_EXTENDED) = 1;
  CONFIG_STATUS(FUJICMD_GET_DIRECTORY_POSITION) = 1;
  CONFIG_STATUS(FUJICMD_GET_HOST_PREFIX) = 1;
  CONFIG_STATUS(FUJICMD_GET_SCAN_RESULT) = 1;
  CONFIG_STATUS(FUJICMD_GET_SSID) = 1;
  CONFIG_STATUS(FUJICMD_GET_WIFISTATUS) = 1;
  CONFIG_STATUS(FUJICMD_GET_WIFI_ENABLED) = 1;
  CONFIG_STATUS(FUJICMD_READ_DEVICE_SLOTS) = 1;
  CONFIG_STATUS(FUJICMD_READ_DIR_ENTRY) = 1;
  CONFIG_STATUS(FUJICMD_READ_HOST_SLOTS) = 1;
  CONFIG_STATUS(FUJICMD_SCAN_NETWORKS) = 1;
  CONFIG_STATUS(FUJICMD_STATUS) = 1;
  CONFIG_STATUS(FUJICMD_READ_APPKEY) = 1;

  did_status_init = 1;
  return;
}

bool fuji_bus_call(uint8_t device, uint8_t unit, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx = 0;
  uint8_t unit_id = sp_fuji_id;


  if (!did_status_init)
    status_init();

  if (sp_get_fuji_id() == 0) {
    fn_device_error = FN_ERR_OFFLINE;
    return false;
  }

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    unit_id = sp_network;
    sp_nw_unit = unit;
  }

  if (fields || !CONFIG_STATUS(fuji_cmd)) {
    if (fields & FUJI_FIELD_AUX1)
      fb_packet->data[idx++] = aux1;
    if (fields & FUJI_FIELD_AUX2)
      fb_packet->data[idx++] = aux2;
    if (fields & FUJI_FIELD_AUX3)
      fb_packet->data[idx++] = aux3;
    if (fields & FUJI_FIELD_AUX4)
      fb_packet->data[idx++] = aux4;
    if (data) {
      memcpy(&fb_packet->data[idx], data, data_length);
      idx += data_length;
    }

    fb_packet->length = idx;

    sp_error = unit_id == sp_network ?
      sp_control_nw(unit_id, fuji_cmd) : sp_control(unit_id, fuji_cmd);
  }

  if (!sp_error && CONFIG_STATUS(fuji_cmd)) {
    sp_error = unit_id == sp_network ?
      sp_status_nw(unit_id, fuji_cmd) : sp_status(unit_id, fuji_cmd);
    if (!sp_error && reply)
      memcpy(reply, &sp_payload[0], reply_length);
  }

  fn_device_error = fn_error(sp_error);
  return !sp_error;
}

uint16_t fuji_bus_read(uint8_t device, uint8_t unit, void *buffer, size_t length)
{
  uint16_t err;
  uint8_t unit_id = sp_fuji_id;


  if (!length)
    return 0;

  if (sp_get_fuji_id() == 0)
    return false;

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    unit_id = sp_network;
    sp_nw_unit = unit;
  }

  length = MIN(length, MAX_TRANSFER_SIZE);

  if (unit_id == sp_network)
    err = sp_read_nw(unit_id, length);
  else
    err = sp_read(unit_id, length);
  if (err)
    return 0;

  memcpy(buffer, &sp_payload[0], length);
  return length;
}

uint16_t fuji_bus_write(uint8_t device, uint8_t unit, const void *buffer, size_t length)
{
  uint16_t err;
  uint8_t unit_id = sp_fuji_id;


  if (!length)
    return 0;

  if (sp_get_fuji_id() == 0)
    return false;

  if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST) {
    unit_id = sp_network;
    sp_nw_unit = unit;
  }

  length = MIN(length, MAX_TRANSFER_SIZE);
  memcpy(&sp_payload[0], buffer, length);

  if (unit_id == sp_network)
    err = sp_write_nw(unit_id, length);
  else
    err = sp_write(unit_id, length);
  if (err)
    return 0;

  return length;
}

bool fuji_error(void)
{
  return fn_device_error != FN_ERR_OK;
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
  fn_device_error = fn_error(sp_error);
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
