#include <fujinet-fuji.h>
#ifndef _CMOC_VERSION_
#include <sys/stat.h>
#include <string.h>
#else
typedef uint32_t time_t;
struct stat {
  size_t st_size;
  time_t st_mtime;
};
#endif /* _CMOC_VERSION_ */

// FujiNet answers nothing until a copy ends. 10 s plus 0.5 s per KB
// covers round trips up to ~150 ms.
#define COPY_WAIT_SECONDS(size) ((uint16_t) (10 + (size) / 2048))

static char src[MAX_FILENAME_LEN];

bool fuji_copy_file_common(uint8_t src_slot, uint8_t dest_slot, const char *copy_spec)
{
  const char *dest;
  uint16_t src_len, seconds, waited;
  uint32_t size;
  FujiDirEntry *entry;
  FNStatus fuji_status;


  dest = strchr(copy_spec, '|');
  if (!dest)
    return false;
  src_len = dest - copy_spec;
  strncpy(src, copy_spec, src_len);
  src[src_len] = 0;
  dest++;

  // Stat now, while FujiNet still answers
  entry = fuji_file_stat(src_slot - 1, src);
  if (!entry)
    return false;
  size = entry->size;
  seconds = COPY_WAIT_SECONDS(size);

  if (FUJICALL_A1_A2_D(FUJICMD_COPY_FILE, src_slot, dest_slot, copy_spec,
                       MAX_FILENAME_LEN))
    return true;

  for (waited = 0; waited < seconds; waited++) {
    if (fuji_status(&fuji_status))
      break;
    wait_jiffies(60);
  }

  // Answering at once means the copy was refused
  if (waited == 0 || waited >= seconds)
    return false;

  entry = fuji_file_stat(dest_slot - 1, dest);
  return entry && entry->size == size;
}
