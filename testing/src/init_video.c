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
  if (isCoCo3)
    width(80);
#elif defined(BUILD_APPLE2)
  videomode(VIDEOMODE_80COL);
#endif // _CMOC_VERSION_
}
