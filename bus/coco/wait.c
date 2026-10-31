#include <fujinet-fuji.h>
#include <coco.h>

void wait_jiffies(uint16_t jiffies)
{
  setTimer(0);
  while (getTimer() < jiffies)
    ;
}
