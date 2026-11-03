#include "fujinet-bus-coleco.h"
#include "fujinet-commands.h"


volatile uint8_t fn_sink;
uint16_t fuji_bus_call_rlen;

/* The SIO-era globals other targets have, so clients compile unchanged.
   fn_device_error is the cart's error, FN_EWAIT, or 144 on a NAK. */
uint8_t fn_default_timeout = 15;
uint8_t fn_device_error;

/*
  A register write is exactly two reads, back to back: one to arm the register
  number, one to deliver the value. The cartridge disarms after a single use,
  so a stray read of the data page on its own can never change anything --
  which is what makes the vblank NMI harmless here. The NMI can land between
  these two lines; the armed register simply waits.
*/
void fn_regwr(uint8_t reg, uint8_t val)
{
  FN_TOUCH(FN_REGSEL + reg);
  FN_TOUCH(FN_REGDAT + val);
}

/*
  Launch the transaction and wait for the cartridge to publish a reply.

  The sequence number comes from the cartridge's own ACKSEQ + 1, never from a
  variable of ours. A console RESET restarts the program and re-zeroes
  everything it owns but does NOT reset the cartridge, so a locally derived
  sequence replays a number the cart has already acknowledged: no request is
  sent, and the stale reply still sitting in the window looks like success.

  The wait loop is deliberately dumb. The cart's own transaction budget is 5s,
  and 60s for MOUNT_IMAGE, so we have to outlast that for a real timeout to
  surface as the cart's error code rather than ours -- but we cannot use a
  timer, because on this machine the timers ride the vblank NMI and a caller
  that has not installed a handler would wait forever on them.
*/
uint8_t fn_commit(void)
{
  uint8_t want = (uint8_t) (FN_ACKSEQ + 1);
  uint16_t outer, inner;


  if (want == 0)
    want = 1;                   /* 0 means "never used" */
  fn_regwr(FNR_SEQ, want);

  for (outer = 0; outer < 2400u; outer++) {
    for (inner = 0; inner < 250u; inner++) {
      if (FN_ACKSEQ == want)
        return FN_ERRCODE;
    }
  }

  return FN_EWAIT;
}

/*
  The FUJI_FIELD_* descriptor maps straight onto the mailbox's parameter
  stream. fuji_field_numbytes() gives the total aux bytes and
  fuji_field_numfields() how many parameters they make up, so their quotient is
  each parameter's size -- always 1, 2 or 4, which is exactly what the stream
  accepts. The DEVCALL_* macros already split wider values low byte first on
  this target (see NATIVE_SPLIT_U16 in fujinet-endian.h), so the aux bytes
  stream through in argument order with no reordering.

  This takes the four aux bytes explicitly rather than through varargs, as adam,
  lynx and msdos do. sccz80 lays a variadic callee's named parameters out as if
  only those had been pushed, but emits calls that push every argument left to
  right, so with DEVCALL_RV's five arguments the callee reads `device` off the
  slot the caller filled with `fields`. The mailbox faithfully forwarded the
  resulting dev=0x10 cmd=0x2F to the ESP32.

  `buf` is the outgoing payload when FUJI_FIELD_DATA is set and the reply
  destination when FUJI_FIELD_REPLY is; no call sets both.
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

  /* Each parameter costs its size byte plus its value bytes. Refuse rather
     than truncate: a short SET_DEVICE_FULLPATH is rejected on the ESP32 side
     anyway, and silently dropping the tail would be far harder to see. */
  if ((fields & FUJI_FIELD_DATA)
      && (uint16_t) (numfields + numbytes) + buf_length > FN_TX_MAX) {
    fn_device_error = FN_ETOOBIG;
    return false;
  }

  fn_regwr(FNR_DATA_RST, 0);    /* rewind the TX write pointer */
  fn_regwr(FNR_DEVICE, device);
  fn_regwr(FNR_CMD, fuji_cmd);
  fn_regwr(FNR_NPARAM, 0);

  idx = 0;
  for (field = 0; field < numfields; field++) {
    FN_TOUCH(FN_TXPAGE + size);
    for (i = 0; i < size; i++)
      FN_TOUCH(FN_TXPAGE + aux[idx++]);
  }
  if (numfields)
    fn_regwr(FNR_NPARAM, numfields);

  if (fields & FUJI_FIELD_DATA) {
    const uint8_t *data = (const uint8_t *) buf;
    size_t n = buf_length;

    while (n--)
      FN_TOUCH(FN_TXPAGE + *data++);
  }

  fn_device_error = fn_commit();
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
    /* Index the window through a local pointer, never through the FN_REPLY
       macro's cast constant: sccz80 indexes the two differently and gets the
       macro form wrong -- the identical expression copies correctly through a
       local and reads back zero through FN_REPLY[i]. */
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
