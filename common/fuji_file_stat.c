#include <fujinet-fuji.h>
#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

static FujiDirEntry entry;
static char dir_path[MAX_FILENAME_LEN]; // FIXME - use shared buffer

FujiDirEntry *fuji_file_stat(uint8_t host, const char *path)
{
  const char *fn_ptr;
  bool found;
  size_t dir_len, entry_len;


  fn_ptr = strrchr(path, '/');
  if (!fn_ptr) {
    fn_ptr = path;
    entry_len = 0;
  }
  else {
    entry_len = fn_ptr - path;
    fn_ptr ++;
  }

  dir_path[0] = 0;
  if (path[0] != '/')
    strcat(dir_path, "/");
  dir_len = strlen(dir_path);
  strncat(dir_path, path, entry_len);
  dir_len += entry_len;
  dir_path[dir_len] = 0;
  dir_path[dir_len + 1] = 0;

  fuji_mount_host_slot(host);
  if (!fuji_open_directory(host, dir_path))
    return NULL;

  found = false;
  // Only read enough data to compare to the filename part of path
  entry_len = sizeof(entry) - sizeof(entry.filename) + strlen(fn_ptr) + 1;
  while (!found) {
    if (!fuji_read_directory((uint8_t) entry_len, FUJI_DIR_FLAG_ADDITIONAL_DATA, &entry))
      break;

    if (entry.modified.month == FUJI_DIR_EOF)
      break;

    if (strcmp(entry.filename, fn_ptr) == 0) {
      found = true;
      break;
    }
  }

  fuji_close_directory();
  if (found)
    return &entry;

  return NULL;
}
