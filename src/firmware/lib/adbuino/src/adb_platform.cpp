//---------------------------------------------------------------------------
//
//	ADBuino ADB keyboard and mouse adapter
//
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2021-2022 akuker
//
//  This file is part of the ADBuino project.
//
//  This file is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
//  any later version.
//
//  This file is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with the file. If not, see <https://www.gnu.org/licenses/>.
//
//---------------------------------------------------------------------------
#include "adb_platform.h"

void AdbInterfacePlatform::Init()
{
  // Set ADB line as input
  adb_pin_in();
}

extern volatile bool adb_collision;
extern volatile bool collision_detection;
static void isr_adb_data_falling_edge(void)
{
  if (collision_detection)
  {
    adb_collision = true;
  }
}

void AdbInterfacePlatform::adb_irq_init(void) 
{
  attachInterrupt(digitalPinToInterrupt(ADB_DATA_ARDUINO_PIN_NUM), isr_adb_data_falling_edge, FALLING);
}

void AdbInterfacePlatform::adb_irq_disable(void) 
{
  detachInterrupt(digitalPinToInterrupt(ADB_DATA_ARDUINO_PIN_NUM));
}