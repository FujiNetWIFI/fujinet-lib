#include <fujinet-fuji.h>
#ifndef _CMOC_VERSION_
#include <sys/stat.h>
#include <string.h>
#else
typedef uint8_t uint_fast8_t;
typedef uint32_t time_t;
struct stat {
  size_t st_size;
  time_t st_mtime;
};
#endif /* _CMOC_VERSION_ */

#define MAX_COPY_RETRIES 100

static char src[MAX_FILENAME_LEN];

bool fuji_copy_file_common(uint8_t src_slot, uint8_t dest_slot, const char *copy_spec)
{
  bool success;
  const char *dest;
  uint16_t src_len;
  uint32_t size;
  uint_fast8_t retries;
  FNStatus fuji_status;
  FujiDirEntry *entry;


  success = FUJICALL_A1_A2_D(FUJICMD_COPY_FILE, src_slot, dest_slot, copy_spec,
                             MAX_FILENAME_LEN);
  if (success)
    return success;

  // Genuinely failed or it timed out because the copy took too
  // long. Send a status command and see if FujiNet is responding.
  for (retries = 0; retries < MAX_COPY_RETRIES; retries++) {
    if (fuji_status(&fuji_status))
      break;
  }

  // Either FujiNet responded on first retry so failure above was
  // genuine failure or we're giving up
  if (retries == 0 || retries >= MAX_COPY_RETRIES)
    return false;

  // Extract src and dest because we might need them later to validate copy succeeded
  dest = strchr(copy_spec, '|');
  if (!dest)
    return false;
  src_len = dest - copy_spec;
  strncpy(src, copy_spec, src_len);
  src[src_len] = 0;
  dest++;

  // Check if file exists and is the same size as the source
  entry = fuji_file_stat(src_slot - 1, src);
  if (!entry)
    return false;

  size = entry->size;
  entry = fuji_file_stat(dest_slot - 1, dest);
  if (!entry)
    return false;

  if (entry->size == size)
    return true;

  return false;
}
