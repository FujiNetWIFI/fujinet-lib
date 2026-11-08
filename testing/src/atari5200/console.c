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

/* The screen does not scroll: text stops one row short, and when that is full the last row
 * asks for a key and the page starts again at the top. conio's LF only moves down a row, so
 * a newline sends the cursor to column 0 first. */
int __fastcall__ write(int fd, const void *buf, unsigned count)
{
  const char *p = (const char *) buf;
  unsigned n;
  uint8_t cols, rows;

  (void) fd;
  screensize(&cols, &rows);
  for (n = 0; n < count; n++) {
    if (wherey() >= rows - 2 && (p[n] == '\n' || wherex() >= cols - 1)) {
      gotoxy(0, rows - 1);
      cputs("-- press a key --");
      cgetc();
      clrscr();
      if (p[n] == '\n')
        continue;
    }
    if (p[n] == '\n')
      cputc('\r');
    cputc(p[n]);
  }

  return (int) count;
}
