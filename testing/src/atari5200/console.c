#include <conio.h>
#include <stdint.h>
#include <unistd.h>

/* cc65's atari5200 target has conio output but no stdout or keyboard: printf needs write(), the harness needs cgetc(). */

#define SKSTAT   (*(volatile uint8_t *) 0xE80F)
#define KEY_DOWN 0x04 /* clear while a keypad key is held */

/* Any keypad key: returns '\r', once the key was let go first. */
char cgetc(void)
{
  while (!(SKSTAT & KEY_DOWN))
    ;
  while (SKSTAT & KEY_DOWN)
    ;

  return '\r';
}

/* The screen does not scroll: when it is full, wait for a key and start again at the top. */
int __fastcall__ write(int fd, const void *buf, unsigned count)
{
  const char *p = (const char *) buf;
  unsigned n;
  uint8_t cols, rows;

  (void) fd;
  screensize(&cols, &rows);
  for (n = 0; n < count; n++) {
    if (wherey() >= rows - 1 && p[n] == '\n') {
      cputs("\n-- press a key --");
      cgetc();
      clrscr();
      continue;
    }
    cputc(p[n]);
  }

  return (int) count;
}
