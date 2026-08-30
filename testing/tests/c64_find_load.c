#ifdef BUILD_C64

#include <stdio.h>
#include <conio.h>
#include <string.h>

#define CURSOR_Y         (*(volatile unsigned char*)0xD6)
#define LINE_LINK_TABLE  ((volatile unsigned char*)0xD9)
#define INPUT_BUFFER     ((volatile unsigned char*)0x0200)

// Storage buffers for our forensic data
static unsigned char fragment[64];
static unsigned char logical_line_text[88]; // Holds up to two physical rows + null
unsigned char c64_load_command[88];

// Convert a C64 Screen Code byte into a standard character byte
static unsigned char screen_to_char(unsigned char code)
{
  // Screen 0 (@) -> PETSCII 64, Screen 1 (A) -> PETSCII 65
  if (code <= 31)
    return code + 64;

  // Screen 32 (Space) -> PETSCII 32, Screen 34 (") -> PETSCII 34
  if (code <= 63)
    return code;

  // Shifted graphic character block
  if (code <= 95)
    return code + 128;

  // Remaining graphic block
  if (code <= 127)
    return code + 64;

  // Fallback default to space character
  return 32;
}

int c64_save_load_command()
{
  int current_row;
  int i, j;
  int found_match = 0;


  c64_load_command[0] = 0;

  for (i = 0; i < 60; i++) {
    unsigned char b = INPUT_BUFFER[4 + i];
    if (b == 0 || b < 32 || b > 126) {
      fragment[i] = '\0';
      break;
    }
    fragment[i] = b;
  }
  fragment[i] = '\0';

  current_row = CURSOR_Y;
  while (current_row >= 0) {
    int start_row = current_row;
    int num_rows = 1;
    int total_chars = 0;

    // If this row is a wrapped continuation line (bit 7 is 0),
    // walk up until we find the physical row where the line actually started.
    while (start_row > 0 && (LINE_LINK_TABLE[start_row] & 0x80) == 0) {
      start_row--;
      num_rows++;
    }

    // Limit logical lines to a maximum of 2 wrapped screen rows (80 characters)
    if (num_rows > 2) num_rows = 2;

    // Construct the full multi-line logical text into logical_line_text[]
    for (i = 0; i < num_rows; i++) {
      unsigned char* screen_ptr = (unsigned char*)(1024 + ((start_row + i) * 40));
      for (j = 0; j < 40; j++) {
        unsigned char raw_code = screen_ptr[j] & 0x7F;
        logical_line_text[total_chars++] = screen_to_char(raw_code);
      }
    }
    logical_line_text[total_chars] = '\0';

    // Trim leading blanks
    for (i = 0; i < total_chars; i++) {
      if (logical_line_text[i] != ' ') {
        if (i)
          memmove(logical_line_text, &logical_line_text[i], total_chars - i);
        break;
      }
    }

    // Trim trailing blank spaces from the end of our combined logical line
    for (i = total_chars - 1; i >= 0; i--) {
      if (logical_line_text[i] != ' ') {
        logical_line_text[i + 1] = '\0';
        break;
      }
    }

    if (strncmp((char*) logical_line_text, "load", 4) == 0) {
      if (strstr((char*) logical_line_text, (char*) fragment) != NULL) {
        strcpy((char*) c64_load_command, (char*) logical_line_text);
        found_match = 1;
        break;
      }
    }

    current_row = start_row - 1;
  }

  return found_match;
}

#endif /* BUILD_C64 */
