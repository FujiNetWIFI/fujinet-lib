#include "fujinet-bus-atari5200.h"
#include "fujinet-commands.h"

uint16_t fuji_bus_call_rlen;

/* fujinet-lib's SIO-era globals, kept so clients written against them
   compile unchanged. fn_default_timeout is in seconds; fn_device_error is
   the cart's error, FN_EWAIT, or 144 (the SIO "device error") on a NAK. */
uint8_t fn_default_timeout = 15;
uint8_t fn_device_error;

#define RTCLOKL (*(volatile uint8_t *) 0x02)  /* the VBI counts frames here */

/*
  The sequence number is the cart's own ACKSEQ + 1, never a variable of
  ours: the console restarts without the cart, and a replayed number would
  be ignored with the old reply still in the window. The wait is counted in
  frames, by the VBI's RTCLOK. Never below 6 s, so the cart's ordinary 5 s
  budget runs out first; fuji_bus_call() waits longer for the calls the
  cart gives longer.
*/
static uint8_t commit(uint8_t secs)
{
  uint8_t want = (uint8_t) (FN_ACKSEQ + 1);
  uint16_t frames;
  uint8_t last, now;


  frames = (uint16_t) (secs < 6 ? 6 : secs) * 60u;
  if (want == 0)
    want = 1;                   /* 0 means "never used" */
  last = RTCLOKL;
  fn_regwr(FNR_SEQ, want);

  while (FN_ACKSEQ != want) {
    now = RTCLOKL;
    if (now != last) {
      last = now;
      if (--frames == 0)
        return FN_EWAIT;
    }
  }

  return FN_ERRCODE;
}

uint8_t fn_commit(void)
{
  return commit(fn_default_timeout);
}

/* At least a second past the cart's own budget for the call (fujimail.c's
   txn_timeout()), so its timeout is the one reported. */
static uint8_t call_timeout(uint8_t device, uint8_t cmd)
{
  uint8_t secs = fn_default_timeout;

  if (cmd == FUJICMD_MOUNT_IMAGE || cmd == FUJICMD_COPY_FILE) {
    if (secs < 61)
      secs = 61;
  }
  else if (device >= FUJI_DEVICEID_NETWORK && device <= FUJI_DEVICEID_NETWORK_LAST
           && (cmd == NETCMD_OPEN || cmd == NETCMD_CLOSE
               || cmd == NETCMD_STATUS || cmd == NETCMD_READ)) {
    if (secs < 91)
      secs = 91;
  }
  return secs;
}

/*
  Fixed four-aux signature (fujinet-bus.h's default): each parameter's
  size is fuji_field_numbytes() / fuji_field_numfields(), always 1, 2 or 4,
  which is exactly what the TX stream takes. `buf` is the payload with
  FUJI_FIELD_DATA and the reply destination with FUJI_FIELD_REPLY.
*/
bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                   const void *buf, size_t buf_length)
{
  uint8_t aux[4];
  uint8_t numbytes, numfields, size;
  uint8_t idx, field, i;
  uint16_t rlen;


  fuji_bus_call_rlen = 0;

  aux[0] = aux1;
  aux[1] = aux2;
  aux[2] = aux3;
  aux[3] = aux4;

  numbytes = fuji_field_numbytes(fields);
  numfields = fuji_field_numfields(fields);
  size = numfields ? (uint8_t) (numbytes / numfields) : 0;

  if ((fields & FUJI_FIELD_DATA)
      && (uint16_t) (numfields + numbytes) + buf_length > FN_TX_MAX) {
    fn_device_error = FN_ETOOBIG;
    return false;
  }

  fn_regwr(FNR_DATA_RST, 0);
  fn_regwr(FNR_DEVICE, device);
  fn_regwr(FNR_CMD, fuji_cmd);
  fn_regwr(FNR_NPARAM, 0);

  idx = 0;
  for (field = 0; field < numfields; field++) {
    fn_tx(size);
    for (i = 0; i < size; i++)
      fn_tx(aux[idx++]);
  }
  if (numfields)
    fn_regwr(FNR_NPARAM, numfields);

  if (fields & FUJI_FIELD_DATA) {
    const uint8_t *data = (const uint8_t *) buf;
    size_t n = buf_length;

    while (n--)
      fn_tx(*data++);
  }

  fn_device_error = commit(call_timeout(device, fuji_cmd));
  if (fn_device_error != FN_OK)
    return false;

  if (FN_REPLYCMD != FUJICMD_ACK) {
    fn_device_error = 144;
    return false;
  }

  rlen = (uint16_t) FN_RXLEN_LO | ((uint16_t) FN_RXLEN_HI << 8);
  if (rlen > FN_REPLY_MAX)
    rlen = FN_REPLY_MAX;

  if (fields & FUJI_FIELD_REPLY) {
    uint8_t *reply = (uint8_t *) buf;
    volatile uint8_t *src = FN_REPLY;
    uint16_t n;

    if (rlen > buf_length)
      rlen = buf_length;
    for (n = rlen; n; n--)
      *reply++ = *src++;
  }

  fuji_bus_call_rlen = rlen;

  return true;
}
