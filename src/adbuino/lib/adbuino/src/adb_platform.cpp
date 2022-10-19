#include "adb_platform.h"

void AdbInterfacePlatform::Init()
{
  // Set ADB line as input
  adb_pin_in();
}
