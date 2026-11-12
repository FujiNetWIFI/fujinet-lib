#include "fujinet-bus-coleco.h"
#include "fujinet-commands.h"


volatile uint8_t fn_sink;
uint16_t fuji_bus_call_rlen;

/* Two reads: arm the register, then deliver the value. The cart disarms after one use, so a vblank NMI between them is harmless. */
void fn_regwr(uint8_t reg, uint8_t val)
{
  FN_TOUCH(FN_REGSEL + reg);
  FN_TOUCH(FN_REGDAT + val);
}

/* The sequence comes from the cart's ACKSEQ, not a local: a console RESET does not reset the cart.
   Budget outlasts the cart's longest (90 s network, 60 s MOUNT/COPY): ~142 T-states per pass. A timer would ride the NMI. */
uint8_t fn_commit(void)
{
  uint8_t want = (uint8_t) (FN_ACKSEQ + 1);
  uint16_t outer, inner;


  if (want == 0)
    want = 1;                   /* 0 means "never used" */
  fn_regwr(FNR_SEQ, want);

  for (outer = 0; outer < 9200u; outer++) {
    for (inner = 0; inner < 250u; inner++) {
      if (FN_ACKSEQ == want)
        return FN_ERRCODE;
    }
  }

  return FN_EWAIT;
}

/* Aux bytes are passed explicitly, not as varargs: sccz80 reads a variadic callee's named parameters from the wrong slots.
   Parameter size is numbytes / numfields (1, 2 or 4). `buf` is the payload for FUJI_FIELD_DATA or the reply target for FUJI_FIELD_REPLY. */
bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                   const void *buf, size_t buf_length)
{
  uint8_t aux[4];
  uint8_t numbytes, numfields, size;
  uint8_t idx, field, i;
  uint16_t rlen;
  uint8_t err;

  fuji_bus_call_rlen = 0;

  aux[0] = aux1;
  aux[1] = aux2;
  aux[2] = aux3;
  aux[3] = aux4;

  numbytes = fuji_field_numbytes(fields);
  numfields = fuji_field_numfields(fields);
  size = numfields ? (uint8_t) (numbytes / numfields) : 0;

  /* Refuse rather than truncate an oversized payload. */
  if ((fields & FUJI_FIELD_DATA)
      && (uint16_t) (numfields + numbytes) + buf_length > FN_TX_MAX) {
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

  err = fn_commit();
  if (err != FN_OK)
    return false;

  if (FN_REPLYCMD != FUJICMD_ACK)
    return false;

  rlen = (uint16_t) FN_RXLEN_LO | ((uint16_t) FN_RXLEN_HI << 8);
  if (rlen > FN_REPLY_MAX)
    rlen = FN_REPLY_MAX;

  if (fields & FUJI_FIELD_REPLY) {
    uint8_t *reply = (uint8_t *) buf;
    /* A local pointer, not FN_REPLY[i]: sccz80 reads the macro form back as zero. */
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
