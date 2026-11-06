/* For programs separate from the FujiNet.  Code built *into* an implementation
 * must define its own fuji_unapi_call(): an inter-slot call switches slots but
 * not ROM banks, so it would land in whichever bank happens to be selected.
 */

#include "fujinet-unapi-msx.h"

#include <string.h>

/* Two sizes because a cartridge has its strings in ROM, so short transfers
   bounce on every call, and its stack is only a few hundred bytes. */
#ifndef FUJI_UNAPI_SMALL_BOUNCE
#define FUJI_UNAPI_SMALL_BOUNCE 256
#endif
#ifndef FUJI_UNAPI_BOUNCE_SIZE
#define FUJI_UNAPI_BOUNCE_SIZE 1024
#endif

/* CALSLT maps the implementation into page 1 and the FujiNet pages its window
   into page 2. */
#define IN_SWITCHED_PAGE(a) \
  ((uint16_t) (a) >= 0x4000 && (uint16_t) (a) < 0xC000)

/* Checked because bouncing onto a stack outside page 3 would fail silently. */
#define STACK_IS_SAFE(a) ((uint16_t) (a) >= 0xC000)

static uint8_t unapi_result;
static uint8_t call_slot;
static void *call_entry;
static uint8_t call_func;
static void *call_arg;

/* Arguments come from memory so that nothing here depends on the compiler's
   calling convention.  The routine returns 1 in L if ok, 0 if not. */
static uint8_t UNAPICall(void)
{
  __asm
    push ix
    push iy

    ld   a, (_call_func)
    ld   hl, (_call_arg)

    push af
    ld   ix, (_call_entry)
    ld   a, (_call_slot)
    push af
    pop  iy             ; IYh = slot; CALSLT ignores IYl
    pop  af

    call 0x001C         ; CALSLT

    ld   a, l           ; only L carries the result; H is undefined
    ld   (_unapi_result), a

    pop  iy
    pop  ix
    __endasm;

  return unapi_result;
}

static uint8_t enter(uint8_t func, FujiNetParams *params)
{
  call_func = func;
  call_arg = params;
  return UNAPICall();
}

/* The implementation may have changed the block, but the buffer pointer was
   ours. */
static void unbounce(FujiNetParams *params, FujiNetParams *local, void *buffer)
{
  memcpy(params, local, sizeof(*params));
  params->buffer = buffer;
}

/* BSS is page 3 in a ROM but follows the program into page 1 in an MSX-DOS
   .COM of any size. */
static uint8_t bounce_block(uint8_t func, FujiNetParams *params)
{
  FujiNetParams local;
  uint8_t ok;

  if (!STACK_IS_SAFE(&local))
    return enter(func, params); /* nowhere better to put it */

  memcpy(&local, params, sizeof(local));
  ok = enter(func, &local);
  unbounce(params, &local, params->buffer);
  return ok;
}

/* The scratch is the caller's, so that it chooses the frame size. */
static uint8_t bounce_via(uint8_t func, FujiNetParams *params, uint8_t *scratch)
{
  FujiNetParams local;
  void *buffer = params->buffer;
  uint16_t length = params->length;
  uint8_t ok;

  if (!STACK_IS_SAFE(&local) || !STACK_IS_SAFE(scratch))
    return 0;

  memcpy(&local, params, sizeof(local));
  local.buffer = scratch;

  if (func == FUJI_CALL_WRITE)
    memcpy(scratch, buffer, length);

  ok = enter(func, &local);

  if (ok && func == FUJI_CALL_READ)
    memcpy(buffer, scratch, local.length <= length ? local.length : length);

  unbounce(params, &local, buffer);
  return ok;
}

static uint8_t bounce_small(uint8_t func, FujiNetParams *params)
{
  uint8_t scratch[FUJI_UNAPI_SMALL_BOUNCE];

  return bounce_via(func, params, scratch);
}

static uint8_t bounce_large(uint8_t func, FujiNetParams *params)
{
  uint8_t scratch[FUJI_UNAPI_BOUNCE_SIZE];

  return bounce_via(func, params, scratch);
}

uint8_t fuji_unapi_call(uint8_t func, FujiNetParams *params)
{
  call_entry = fuji_unapi_entry();
  if (!call_entry)
    return 0;
  call_slot = fuji_unapi_slot();

  /* The implementation would read its own ROM instead of this buffer, and the
     reply would look like device nonsense. */
  if (params->length && params->buffer && IN_SWITCHED_PAGE(params->buffer)) {
    if (params->length <= FUJI_UNAPI_SMALL_BOUNCE)
      return bounce_small(func, params);
    if (params->length <= FUJI_UNAPI_BOUNCE_SIZE)
      return bounce_large(func, params);
    return 0;
  }

  return bounce_block(func, params);
}
