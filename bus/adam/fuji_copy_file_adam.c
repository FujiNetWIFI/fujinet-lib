#include "fujinet-bus-adam.h"
#include "fujinet-fuji.h"
#include <string.h>

// FujiNet answers nothing until a copy ends. 10 s plus 0.5 s per KB
// covers round trips up to ~150 ms.
#define COPY_WAIT_SECONDS(size) (10 + ((uint16_t) ((size) >> 8) >> 3))

static char src[MAX_FILENAME_LEN];

bool fuji_copy_file_adam(uint8_t src_slot, uint8_t dest_slot, const char *copy_spec)
{
  const char *dest;
  uint16_t src_len, seconds;
  FujiDirEntry *entry;
  DCB *dcb;


  dest = strchr(copy_spec, '|');
  if (!dest)
    return false;
  src_len = dest - copy_spec;
  strncpy(src, copy_spec, src_len);
  src[src_len] = 0;

  // Stat now, while FujiNet still answers
  entry = fuji_file_stat(src_slot - 1, src);
  if (!entry)
    return false;
  seconds = COPY_WAIT_SECONDS(entry->size);

  if (FUJICALL_A1_A2_D(FUJICMD_COPY_FILE, src_slot, dest_slot, copy_spec,
                       MAX_FILENAME_LEN))
    return true;

  dcb = dcb_find(ADAM_ID_FUJINET);
  if (!dcb)
    return false;

  // A STATUS keeps the failure bit until the next command
  for (; seconds; seconds--) {
    if (bus_ready(dcb, 1))
      return !(dcb->dev_status & ADAM_STATUS_CMD_FAILED);
    wait_jiffies(60);
  }
  return false;
}
