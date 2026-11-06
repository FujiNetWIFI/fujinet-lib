#include "fujinet-bus-nes.h"
#include "fujinet-commands.h"


uint16_t fuji_bus_call_rlen;

void __fastcall__ fn_tx(uint8_t b)
{
  FN_TXPAGE = b;
}

/* One store: the address picks the register, the data is the value. */
void fn_regwr(uint8_t reg, uint8_t val)
{
  FN_REGSEL[reg] = val;
}

/* The sequence comes from the cart's ACKSEQ: a console RESET does not reset the cart.
   The budget outlasts the cart's longest wait (90 s for a network OPEN or CLOSE):
   about 13 ms per outer pass at 1.79 MHz, so 7600 passes is about 100 s. */
uint8_t fn_commit(void)
{
  uint8_t want = (uint8_t) (FN_ACKSEQ + 1);
  uint16_t outer, inner;


  if (want == 0)
    want = 1;                   /* 0 means "never used" */
  fn_regwr(FNR_SEQ, want);

  for (outer = 0; outer < 7600u; outer++) {
    for (inner = 0; inner < 250u; inner++) {
      if (FN_ACKSEQ == want)
        return FN_ERRCODE;
    }
  }

  return FN_EWAIT;
}

/* The aux bytes stream through in argument order, one parameter of numbytes / numfields bytes each
   (always 1, 2 or 4); the DEVCALL_* macros already split wide values low byte first here.
   `buf` is the payload when FUJI_FIELD_DATA is set and the reply destination when FUJI_FIELD_REPLY is. */
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

  /* Each parameter costs its size byte plus its value bytes. Refuse rather than truncate. */
  if ((fields & FUJI_FIELD_DATA)
      && (uint16_t) (numfields + numbytes) + buf_length > FN_TX_MAX)
    return false;

  fn_regwr(FNR_DATA_RST, 0);    /* rewind the TX write pointer */
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

  if (fn_commit() != FN_OK)
    return false;

  if (FN_REPLYCMD != FUJICMD_ACK)
    return false;

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
