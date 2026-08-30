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
    setBorderColor(0);
    attr(0, 0, 0, 0);
    cls(0);
  }
#elif defined(BUILD_APPLE2)
  videomode(VIDEOMODE_80COL);
#elif defined(BUILD_C64)
  c64_save_load_command();
#endif // _CMOC_VERSION_

  return;
}
