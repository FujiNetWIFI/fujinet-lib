/* The stand-off UNAPI back end: enter the FujiNet implementation that
 * fujinet-unapi-extbio.c found, with an inter-slot call.
 *
 * For a program separate from the FujiNet - a cartridge, a .COM, a BASIC
 * extension.  Code built *into* an implementation must define its own
 * fuji_unapi_call() instead: an inter-slot call switches slots but leaves the
 * ROM bank register alone, so entering your own cartridge that way lands at
 * the published address in whichever bank happens to be selected.  This module
 * is pulled out of the library only when nothing else defines that symbol.
 */

#include "fujinet-unapi-msx.h"

#include <string.h>

/* Stack scratch for bouncing a caller's buffer out of a page the
   implementation is about to map itself over.  A caller that keeps its buffers
   where the specification says never reaches this.

   Two sizes because the frame is only free if it is not taken: a cartridge has
   its strings in ROM, so short transfers bounce on every call, and its stack
   is only the few hundred bytes between BSS and HIMEM. */
#ifndef FUJI_UNAPI_SMALL_BOUNCE
#define FUJI_UNAPI_SMALL_BOUNCE 256
#endif
#ifndef FUJI_UNAPI_BOUNCE_SIZE
#define FUJI_UNAPI_BOUNCE_SIZE 1024
#endif

/* The pages an implementation may take over while a call runs: CALSLT maps it
   into page 1 to enter it, and the FujiNet pages its window into page 2.
   Anything it dereferences has to sit outside both. */
#define IN_SWITCHED_PAGE(a) \
  ((uint16_t) (a) >= 0x4000 && (uint16_t) (a) < 0xC000)

/* Bounces come off the stack, so the stack itself has to be page 3.  It is on
   any normal caller; checked rather than assumed, because being wrong here is
   silent. */
#define STACK_IS_SAFE(a) ((uint16_t) (a) >= 0xC000)

static uint8_t unapi_result;
static uint8_t call_slot;
static void *call_entry;
static uint8_t call_func;
static void *call_arg;

/* Enter the implementation through CALSLT, which wants the routine address in
   IX and the slot in IYh.  Arguments come from memory rather than the stack so
   that nothing here depends on the compiler's calling convention.

   In: A = routine number, HL = parameter block.  Out: L = 1 ok, 0 not. */
static uint8_t UNAPICall(void)
{
  __asm
    push ix
    push iy

    ld   a, (_call_func)
    ld   hl, (_call_arg)

    push af             ; the routine number, while A is needed for the slot
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

/* Copy back what the implementation may have changed - all but the buffer
   pointer, which was ours rather than the caller's. */
static void unbounce(FujiNetParams *params, FujiNetParams *local, void *buffer)
{
  memcpy(params, local, sizeof(*params));
  params->buffer = buffer;
}

/* The parameter block alone, moved to the stack.  The common case: the block
   is a static, and BSS is page 3 in a ROM but follows the program into page 1
   in an MSX-DOS .COM of any size. */
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

/* The block and the buffer it points at, for a caller that put its buffer
   somewhere the specification does not allow.  The scratch is the caller's, so
   the frame it costs is chosen there. */
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

  /* A buffer the implementation cannot see would have it read its own ROM
     instead, and the reply then looks like device nonsense rather than a
     caller that broke the rules.  Bounce it if it fits, fail if it does not. */
  if (params->length && params->buffer && IN_SWITCHED_PAGE(params->buffer)) {
    if (params->length <= FUJI_UNAPI_SMALL_BOUNCE)
      return bounce_small(func, params);
    if (params->length <= FUJI_UNAPI_BOUNCE_SIZE)
      return bounce_large(func, params);
    return 0;
  }

  return bounce_block(func, params);
}
