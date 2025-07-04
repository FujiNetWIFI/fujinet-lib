#include "fujinet-bus-apple2.h"
#include "fujinet-fuji.h"
#include <string.h>

#define FUJICMD_HIGHEST FUJICMD_RESET
#define FUJICMD_LOWEST  FUJICMD_STATUS

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
  CONFIG_STATUS(FUJICMD_GET_ADAPTERCONFIG_EXTENDED) = 1;
  CONFIG_STATUS(FUJICMD_GET_HOST_PREFIX) = 1;
  CONFIG_STATUS(FUJICMD_READ_DEVICE_SLOTS) = 1;
  CONFIG_STATUS(FUJICMD_READ_HOST_SLOTS) = 1;
  CONFIG_STATUS(FUJICMD_READ_DIR_ENTRY) = 1;
  did_status_init = 1;
  return;
}

bool fuji_bus_call(uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
{
  uint16_t idx = 0;


  // These arguments are for compatibility with larger systems and not
  // used on Apple II. Hack to suppress the warning that they are
  // unused.
  (void) aux3;
  (void) aux4;

  sp_error = SP_ERR_OK;
  if (!did_status_init)
    status_init();

  if (sp_get_fuji_id() == 0)
    return false;

  if (fields || !CONFIG_STATUS(fuji_cmd)) {
    if (fields & FUJI_FIELD_AUX1)
      fb_packet->data[idx++] = aux1;
    if (fields & FUJI_FIELD_AUX2)
      fb_packet->data[idx++] = aux2;
    if (fields & FUJI_FIELD_DATA) {
      memcpy(&fb_packet->data[idx], data, data_length);
      idx += data_length;
    }

    fb_packet->length = idx;

    sp_error = sp_control(sp_fuji_id, fuji_cmd);
  }

  if (!sp_error && CONFIG_STATUS(fuji_cmd))
    sp_error = sp_status(sp_fuji_id, fuji_cmd);

  if (reply)
    memcpy(reply, &sp_payload[0], reply_length);

  return !sp_error;
}
