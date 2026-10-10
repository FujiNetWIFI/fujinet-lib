#ifndef FUJINET_UNAPI_MSX_H
#define FUJINET_UNAPI_MSX_H

/* The MSX-UNAPI contract for reaching a FujiNet: what an implementation
 * publishes and a client calls.  Shared with the ROM side - the FujiNet MSX
 * UNAPI firmware, and the implementation in the Nextor driver bank - so keep
 * the two in step.
 */

#include <fujinet-int.h>

/* The API identifier an implementation answers to during EXTBIO discovery;
   identifiers are case insensitive. */
#define FUJI_UNAPI_ID "FUJINET"

/* The block FUJI_CALL_WRITE and FUJI_CALL_READ take in HL.  The caller must
 * keep it, and the buffer it points at, out of page 1 and page 2: an
 * implementation is free to page itself in over either while the call runs.
 */
typedef struct {
  uint8_t device;
  uint8_t command;
  uint8_t aux_descr;
  uint8_t aux1, aux2, aux3, aux4;
  void *buffer;
  uint16_t length;
} FujiNetParams;

/* Routine numbers. */
enum {
  FUJI_CALL_INFO  = 0,
  FUJI_CALL_WRITE = 1,
  FUJI_CALL_READ  = 2,
};

/* How fuji_bus_call() reaches the implementation, and the seam that lets one
 * library serve two callers.  A separate program links the back end in
 * fujinet-unapi-extbio.c, which discovers an implementation and enters it with
 * an inter-slot call.  Code built *into* an implementation cannot use that - an
 * inter-slot call switches slots but not ROM banks - and defines this itself,
 * calling the routines directly.  The library module is pulled in only when
 * nothing else defines it.
 *
 * Out: 1 if the device answered, 0 if it did not.
 */
extern uint8_t fuji_unapi_call(uint8_t func, FujiNetParams *params);

#endif /* FUJINET_UNAPI_MSX_H */
