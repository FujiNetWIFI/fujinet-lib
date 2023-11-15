#include <stdint.h>
#include <cbm.h>

#include "fujinet-io.h"
#include "fn_data.h"

// Buffer needs to be at least 1024 bytes
void fn_io_get_device_filename(uint8_t ds, char *buffer)
{
  char c[3];

  c[0] = FUJICMD_GET_DEVICE_FULLPATH;
  c[1] = ds;
  c[2] = 0;

  cbm_open(LFN,DEV,SAN,c);
  cbm_read(LFN,buffer,1024);
  cbm_close(LFN);

}
