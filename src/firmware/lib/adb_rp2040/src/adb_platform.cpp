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
#include "adb.h"
#include "adb_platform.h"
#include "adbkbdparser.h"
#include "rp2040_gpio.h"
#include "hardware/gpio.h"
#include <time.h>

uint32_t millis()
{
  return to_ms_since_boot(get_absolute_time());
}

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



void AdbInterfacePlatform::data_lo()
{
    ADB_OUT_LOW();
}

void AdbInterfacePlatform::data_hi()
{
    ADB_OUT_HIGH();
}
uint8_t AdbInterfacePlatform::data_in()
{
    return ADB_IN_GET();
}

void AdbInterfacePlatform::adb_pin_out()
{
}
void AdbInterfacePlatform::adb_pin_in()
{
}

uint16_t AdbInterfacePlatform::wait_data_lo(uint32_t us)
{
  // use 64bit time to prevent possible wrapping
  uint64_t start = time_us_64();
  uint64_t time;
  do
  {
    if (!data_in()) {
      time = time_us_64();
      break;
    }
    time = time_us_64();
  } while (us >= time - start);
  uint16_t diff = static_cast<uint16_t>(time - start);
  return us >= diff ? diff : 0;
}

uint16_t AdbInterfacePlatform::wait_data_hi(uint32_t us)
{
  uint64_t start = time_us_64();
  uint64_t time;
  do
  {
    if (data_in()) {
      time = time_us_64();
      break;
    }
    time = time_us_64();
  } while (us >= time - start);
  uint16_t diff = static_cast<uint16_t>(time - start);
  return diff;
}


