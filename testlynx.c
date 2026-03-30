#include "fujinet-fuji.h"
#include <stdio.h>
#include <string.h>
#include <tgi.h>


AdapterConfigExtended ace;
char buffer[256];
char s[64];
unsigned char status;

int main()
{
   // Setup TGI
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_setbgcolor(TGI_COLOR_BLACK);
  tgi_clear();

  // setup joystick
  //joy_install(joy_static_stddrv);



                     //012345678901234567890
  tgi_outtextxy(1, 1, "Searching for FN...");
  if (!fuji_get_adapter_config(&ace))
    strcpy(ace.fn_version, "FAIL");

  sprintf(s, "FN: %-14s", ace.fn_version);
  tgi_outtextxy(1, 9, s);


  if (!fuji_get_wifi_status(&status))
    tgi_outtextxy(1, 20, "FAIL:get_wifi_status");
  if (status == connected)
    tgi_outtextxy(1, 20, "Wifi connected!");

  if (!fuji_get_host_prefix(1, &buffer))
    tgi_outtextxy(1,28, "FAIL: get_host_prefix");
  sprintf(s, "Prefix: %s\n", buffer);
  tgi_outtextxy(1, 37, s);


  while(1);
  return 0;
}
