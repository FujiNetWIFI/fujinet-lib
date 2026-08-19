#include "init_video.h"

#ifdef BUILD_APPLE2
#include <apple2.h>
#endif /* BUILD_APPLE2 */

#ifdef BUILD_COCO
#include <coco.h>
#endif /* BUILD_COCO */

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
#endif // _CMOC_VERSION_
}
