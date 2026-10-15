#include "init_video.h"

#ifdef BUILD_APPLE2
#include <apple2.h>
#endif /* BUILD_APPLE2 */

#ifdef BUILD_COCO
#include <coco.h>
#endif /* BUILD_COCO */

#ifdef BUILD_C64
#include "c64_find_load.h"
#endif /* BUILD_C64 */

#if defined(BUILD_PMD85) || defined(BUILD_ADAM)
#include <conio.h>
#endif /* BUILD_PMD85 || BUILD_ADAM */

void init_video()
{
#if defined(BUILD_COCO)
  initCoCoSupport();
  if (isCoCo3) {
    width(80);
    /* Text takes its background from palette slots 0-7 and its foreground
     * from 8-15, so both ends have to be set to override the default. */
    palette(0, 0);
    palette(8, 63);
    /* Yellow and red that read as such on RGB and composite alike; slot 9
     * keeps the ROM's green for the monitor it booted with. */
    palette(10, 0x34);
    palette(12, 0x26);
    setBorderColor(0);
    attr(0, 0, 0, 0);
    cls(0);
  }
#elif defined(BUILD_APPLE2)
  videomode(VIDEOMODE_80COL);
#elif defined(BUILD_C64)
  c64_save_load_command();
#elif defined(BUILD_PMD85)
  clrscr();
#elif defined(BUILD_ADAM)
  /* The bitmap mode gives each character its own colors. */
  conio_set_screen_mode(2);
#endif

  return;
}
