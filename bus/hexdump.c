
#if defined(DEBUG) && defined(HEXDUMP)
#define COLUMNS 16

static void hexdump(void *ptr, int count)
{
  int outer, inner;
  uint8_t c;
  uint8_t *buffer = (uint8_t *) ptr;


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
