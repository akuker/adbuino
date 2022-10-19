#include "adb_platform.h"
#include <time.h>

void AdbInterfacePlatform::Init()
{
  // Set ADB line as input
  adb_gpio_init();
}