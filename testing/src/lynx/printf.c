#include <lynx.h>
#include <tgi.h>
#include <6502.h>
#include <string.h>

// Atari Lynx font is hard-locked at 8x8 pixels.
// 160 pixels / 8 pixel width = 20 columns (0 to 19)
#define MAX_COLS 19
// 102 pixels / 8 pixel height = 12 rows (0 to 11)
#define MAX_ROWS 12

static char screen_buf[MAX_ROWS][MAX_COLS + 1];
static int cursor_row = 0;
static int cursor_col = 0;
static char initialized = 0;

void init_lynx_text_screen(void)
{
  tgi_install(tgi_static_stddrv);
  tgi_init();
  CLI();

  // Note: tgi_settextscale is omitted because the Lynx bitmap font driver ignores it.

  while (tgi_busy());
  tgi_clear();
  initialized = 1;
}

void flush_text_to_screen(void)
{
  int r;
  if (tgi_busy()) return;

  tgi_clear();
  tgi_setcolor(COLOR_WHITE);

  for (r = 0; r < MAX_ROWS; ++r) {
    // Safe check for null-terminated strings at the first array column slot
    if (screen_buf[r][0] != '\0') {
      // Render text lines exactly spaced 8 pixels apart vertically
      tgi_outtextxy(0, r * 8, screen_buf[r]);
    }
  }
  tgi_updatedisplay();
}

int __fastcall__ write(int /*fd*/, const void* buf, unsigned int count)
{
  const char* data = (const char*)buf;
  unsigned int i;

  if (!initialized) {
    init_lynx_text_screen();
  }

  for (i = 0; i < count; ++i) {
    char c = data[i];

    if (c == '\n' || c == '\r') {
      cursor_col = 0;
      cursor_row++;
    }
    else {
      // Hardware boundary wrap: Wrap down automatically at the 20th character
      if (cursor_col >= MAX_COLS) {
        cursor_col = 0;
        cursor_row++;
      }

      if (cursor_row < MAX_ROWS) {
        screen_buf[cursor_row][cursor_col] = c;
        cursor_col++;
        screen_buf[cursor_row][cursor_col] = '\0';
      }
    }

    // Handle text scrolling safely when tests exceed the 12th row
    if (cursor_row >= MAX_ROWS) {
      int r;
      for (r = 0; r < MAX_ROWS - 1; ++r) {
        strcpy(screen_buf[r], screen_buf[r + 1]);
      }
      screen_buf[MAX_ROWS - 1][0] = '\0';
      cursor_row = MAX_ROWS - 1;
    }
  }

  flush_text_to_screen();
  return count;
}
