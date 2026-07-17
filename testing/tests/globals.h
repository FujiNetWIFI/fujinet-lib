#ifndef GLOBALS_H
#define GLOBALS_H

/* =========================================================================
 * Large buffers as globals (avoids blowing the stack)
 * ========================================================================= */

#include <fujinet-fuji.h>

#ifndef MAX_FILENAME_LEN
#define MAX_FILENAME_LEN 256
#endif

/*
 * All large buffers live in a single union.  Tests run sequentially so
 * buffers needed by different test functions can overlap freely.  Where a
 * function needs multiple buffers simultaneously they are wrapped in a
 * nested struct inside the union so they each get distinct addresses.
 *
 * Union member layout:
 *   net          -- generic network read buffer (512 bytes, largest member)
 *   adapter      -- AdapterConfig + AdapterConfigExtended (used sequentially,
 *                   but kept separate because the structs have distinct types)
 *   slots        -- host/host2/device slot arrays (host and host2 must
 *                   coexist for the post-write comparison)
 *   appkey       -- write and read buffers (sequential, but read is 2 bytes
 *                   larger than write so kept separate)
 *   b64          -- base64 encode and decode buffers (sequential)
 *   hash         -- all six hash output buffers; sha256 must survive until
 *                   compared with inc1/inc2 so all six are distinct fields
 *   clock_fmt    -- single time buffer reused across format tests
 *   clock_tz_call-- four buffers that must coexist for the strcmp at the end
 *                   of test_clock_get_time_tz
 *   clock_tz_set -- original + readback TZ strings that must coexist
 *   dir          -- directory path/filter buffer (258 bytes)
 */

#include "constants.h"

typedef union {
  uint8_t net[512];

  struct {
    AdapterConfig         ac;
    AdapterConfigExtended ace;
  } adapter;

  struct {
    HostSlot   hosts[MAX_HOSTS];
    HostSlot   hosts2[MAX_HOSTS];
  } hslots;

  struct {
    DeviceSlot devices[MAX_DISKS];
    DeviceSlot devices2[MAX_DISKS];
  } dslots;

  struct {
    uint8_t write[64];
    uint8_t read[66];
  } appkey;

  struct {
    char enc[32];
    char dec[32];
  } b64;

  union {
    uint8_t md5[33];
    uint8_t sha1[41];
    uint8_t sha256[65];
    uint8_t sha512[129];
  } hash;

  struct {
    uint8_t inc1[65];
    uint8_t inc2[65];
  } hash_verify;

  uint8_t clock_fmt[32];

  struct {
    uint8_t utc[32];
    uint8_t ny[32];
    uint8_t tok[32];
    char    sys_tz[64];
  } clock_tz_call;

  struct {
    char original[64];
    char readback[64];
  } clock_tz_set;

  struct {
    char prefix[MAX_FILENAME_LEN];
    char read_back[MAX_FILENAME_LEN];
  } host_prefix;

  struct {
    char filename[MAX_FILENAME_LEN];
    char read_back[MAX_FILENAME_LEN];
  } device_filename;

  char dir[258];
} buf_union;

extern buf_union g;

#endif /* GLOBALS_H */
