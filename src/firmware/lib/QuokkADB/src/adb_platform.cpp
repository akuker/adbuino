//---------------------------------------------------------------------------
//
//	QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the QuokkaADB project.
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
#include "adbkbdparser.h"
#include "hardware/gpio.h"
#include <time.h>

bool AdbInterfacePlatform::adb_delay_us(uint32_t delay) 
{
  uint64_t start = time_us_64();
  uint64_t time;
  bool collision_free = true;
  do
  {
    if (adb_collision) {
      collision_free = false;
      break;
    }
    time = time_us_64();
  } while (delay >= time - start);
  
  return collision_free;
}

extern bool adb_collision;
extern bool collision_detection;
extern ADBKbdRptParser KeyboardPrs;

static void adb_in_irq_callback(uint gpio, uint32_t event_mask) {
    // a possible collision occurs when ABD is meant to be high
    // but the the bus is set low by another device
    volatile bool gpio_in_low = !gpio_get(ADB_IN_GPIO);
    volatile bool gpio_out_high = gpio_get(ADB_OUT_GPIO);
    if (collision_detection && gpio == ADB_IN_GPIO  && gpio_out_high && gpio_in_low && (event_mask & GPIO_IRQ_EDGE_FALL) ) {
        adb_collision = true;
    } 
}

void AdbInterfacePlatform::adb_irq_init(void) {
  gpio_acknowledge_irq(ADB_IN_GPIO, GPIO_IRQ_EDGE_FALL);
  gpio_set_irq_enabled_with_callback(ADB_IN_GPIO, GPIO_IRQ_EDGE_FALL, true, &adb_in_irq_callback);
}

void AdbInterfacePlatform::adb_irq_disable(void) {
  gpio_set_irq_enabled_with_callback(ADB_IN_GPIO, GPIO_IRQ_EDGE_FALL, false, &adb_in_irq_callback); 
  gpio_acknowledge_irq(ADB_IN_GPIO, GPIO_IRQ_EDGE_FALL);
}

void AdbInterfacePlatform::adb_set_leds(uint16_t reg2) 
{
  bool numlock = !(reg2 & 0x1);
  bool capslock = !(reg2 & 0x2);
  bool scrolllock = !(reg2 & 0x4);
  KeyboardPrs.SetUSBkeyboardLEDs(capslock, numlock, scrolllock);
}
