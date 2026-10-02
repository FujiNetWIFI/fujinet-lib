#include "fujinet-bus-msx.h"
#include "fujinet-commands.h"

#undef HEXDUMP
#if defined(DEBUG) || defined(HEXDUMP)
#define COLUMNS 16

static void hexdump(uint8_t *buffer, int count)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < count; outer += COLUMNS) {
    for (inner = 0; inner < COLUMNS; inner++) {
      if (inner + outer < count) {
	c = buffer[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < COLUMNS && inner + outer < count; inner++) {
      c = buffer[inner + outer];
      if (c >= ' ' && c <= 0x7f)
	printf("%c", c);
      else
	printf(".");
    }
    printf("|\n");
  }

  return;
}
#endif /* HEXDUMP */

#define UNAPI_SERVICE "FUJINET"

//***************
//  FIXME - this stuff belongs in a header file shared with FujiNet MSX UNAPI firmware

typedef struct {
  uint8_t device;
  uint8_t command;
  uint8_t aux_descr;
  uint8_t aux1, aux2, aux3, aux4;
  void *buffer;
  uint16_t length;
} FujiNetParams;

enum {
  FUJI_CALL_INFO = 0,
  FUJI_CALL_WRITE,
  FUJI_CALL_READ,
};

//***************

//static unapi_code_block code_block;
static FujiNetParams params;

static uint8_t unapi_count, unapi_slot, unapi_in_ram;
static uint16_t unapi_result;
static void *unapi_entry;

static uint8_t UNAPIGetCount(const char *service)
{
  volatile char *p;
  volatile char *ARG = (volatile char*) 0xF847;


  for (p = ARG; *service; p++, service++)
    *p = *service;
  *p = 0;

  // Call EXTBIO: DE=$2222, A=0, B=0 -> B=count
  __asm
    ld   de, 0x2222
    xor  a
    ld   b, a
    call 0xFFCA
    ld   a, b
    ld   (_unapi_count), a
    __endasm;
  return unapi_count;
}

static void UNAPIGetSlot(uint8_t index)
{
  // A=index, B=1, DE=0x2222 -> EXTBIO (0xFFCA)
  __asm
    push ix

    ld   de, 0x2222
    ld   b, 1           // Function 1: Get Sliver Info

    // Fetch 'index' from the stack
    // Offset 4: 2 bytes for Return Address + 2 bytes for Pushed IX
    ld   ix, 4
    add  ix, sp
    ld   a, (ix+0)

    call 0xFFCA          // EXTBIO

    // If EXTBIO is successful, A = Slot ID, HL = Entry Point
    ld   (_unapi_slot), a
    ld   (_unapi_entry), hl
    ld   a, b
    ld   (_unapi_in_ram), a

    pop  ix
    __endasm;
}

static uint8_t  call_func;
static void* call_arg;

static uint16_t UNAPICall(uint8_t func, void *arg)
{
  call_func = func;
  call_arg = arg;

  __asm
    push ix
    push iy

    ld   a, (_call_func)
    ld   hl, (_call_arg)

#if 0
    // --- BORDER DEBUG ---
    // If A == 3, Border = White (Color 15)
    // If A == 0, Border = Blue (Color 4)
    // If A == something else, Border = Red (Color 2)
    cp   3
    jr   z, _is_three
    or   a
    jr   z, _is_zero
    ld   a, 8         // Red (Unknown)
    jr   _apply_color
    _is_three:
    ld   a, 3        // Green (Success)
    jr   _apply_color
    _is_zero:
    ld   a, 15         // White (Offset is wrong, likely grabbing high byte)
    _apply_color:
    out  (0x99), a
    ld   a, 0x87
    out  (0x99), a

    hang_here:
    jp hang_here

    // --- END DEBUG ---
#endif

    // 2. Prepare CALSLT ($001C)
    // Register IX = Dynamic Target Address (unapi_entry)
    // Register IYh = Dynamic Slot ID (unapi_slot)

    push af           // Save 'func' while we mess with A

    // Load Dynamic Address into IX
    ld   ix, (_unapi_entry)

    // Load Dynamic Slot into IYh
    ld   a, (_unapi_slot)
    push af
    pop  iy           // Now IYh = Slot ID, IYl = Flags (Flags don't matter for CALSLT)

    pop  af           // Restore 'func' to A

    // 3. Execute BIOS CALSLT
    // A = Function ID
    // HL = Argument
    // IX = Dynamic Entry Point
    // IYh = Slot ID
    call 0x001C

    // 4. Capture 16-bit result from HL
    ld   (_unapi_result), hl

    pop  iy
    pop  ix
    __endasm;

  return unapi_result;
}

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
                   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
                   const void *buf, size_t buf_length)
{
  //Z80_registers regs;
  uint16_t idx, numbytes;
  int count = UNAPIGetCount(UNAPI_SERVICE);


  if (!count)
    return false;

  params.device = device;
  params.command = fuji_cmd;
  params.aux_descr = fields;

  params.aux1 = aux1;
  params.aux2 = aux2;
  params.aux3 = aux3;
  params.aux4 = aux4;

  unapi_slot = 0xFF;
#ifdef DEBUG
  printf("FINDING SLOT\n");
#endif
  UNAPIGetSlot(1);
#ifdef DEBUG
  printf("IN SLOT %d RAM: %d ENTRY: 0x%04x\n", unapi_slot, unapi_in_ram, unapi_entry);
  printf("OUT PARAMS: 0x%04x\n", &params);
#endif

  params.buffer = buf;
  params.length = buf_length;

  if (fields & FUJI_FIELD_REPLY) {
#ifdef DEBUG
    printf("FUJINET READ %d\n", params.length);
    hexdump(&params, sizeof(params));
#endif
    return UNAPICall(FUJI_CALL_READ, &params);
  }

#ifdef DEBUG
  printf("FUJINET WRITE %d\n", params.length);
#endif
  return UNAPICall(FUJI_CALL_WRITE, &params);
}

uint16_t network_bus_read(uint8_t device, void *buffer, size_t length)
{
  NETCALL_B12_RV(FUJICMD_READ, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}

uint16_t network_bus_write(uint8_t device, const void *buffer, size_t length)
{
  NETCALL_B12_D(FUJICMD_WRITE, device - FUJI_DEVICEID_NETWORK + 1, length, buffer, length);
  return length;
}
