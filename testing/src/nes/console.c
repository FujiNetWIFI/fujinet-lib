#include <conio.h>
#include <stdint.h>
#include <unistd.h>

/* cc65's nes target has conio output but no stdout or keyboard: printf needs write(), the harness needs cgetc(). */

#define JOY1 (*(volatile uint8_t *) 0x4016)
#define BTN_A     0x01
#define BTN_START 0x08

static uint8_t joy_read(void)
{
  uint8_t i, bits = 0;

  JOY1 = 1;
  JOY1 = 0;
  for (i = 0; i < 8; i++)
    bits |= (uint8_t) ((JOY1 & 1) << i);

  return bits;
}

/* Any of A or Start: returns '\r', once the buttons were let go first. */
char cgetc(void)
{
  while (joy_read() & (BTN_A | BTN_START))
    ;
  while (!(joy_read() & (BTN_A | BTN_START)))
    ;

  return '\r';
}

/* The screen does not scroll: when it is full, wait for a button and start again at the top. */
int __fastcall__ write(int fd, const void *buf, unsigned count)
{
  const char *p = (const char *) buf;
  unsigned n;
  uint8_t cols, rows;

  (void) fd;
  screensize(&cols, &rows);
  for (n = 0; n < count; n++) {
    if (wherey() >= rows - 1 && p[n] == '\n') {
      cputs("\n-- press A --");
      cgetc();
      clrscr();
      continue;
    }
    cputc(p[n]);
  }

  return (int) count;
}
