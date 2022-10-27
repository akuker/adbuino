
//---------------------------------------------------------------------------
//
//	ADBuino & QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the ADBuino and the QuokkaADB projects.
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
#pragma once

#include <stdint.h>
#include "quokkadb_gpio.h"
extern bool adb_collision;
class AdbInterfacePlatform {
  public:
    void Init();

  protected:

    void data_lo();
    void data_hi();
    void adb_pin_out();
    void adb_pin_in();

    uint8_t data_in();
    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    // delay for set amount unless a collision is detected then return false
    // otherwise return true
    bool adb_delay_us(uint32_t delay);

};



inline void AdbInterfacePlatform::data_lo()
{
    ADB_OUT_LOW();
}

inline void AdbInterfacePlatform::data_hi()
{
    ADB_OUT_HIGH();
}
inline uint8_t AdbInterfacePlatform::data_in()
{
    return ADB_IN_GET();
}

inline void AdbInterfacePlatform::adb_pin_out()
{
}
inline void AdbInterfacePlatform::adb_pin_in()
{
}

inline uint16_t AdbInterfacePlatform::wait_data_lo(uint32_t us)
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
  uint32_t diff = static_cast<uint32_t>(time - start);
  return us >= diff ? us - (time - start) : 0;
}

inline uint16_t AdbInterfacePlatform::wait_data_hi(uint32_t us)
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
  uint32_t diff = static_cast<uint32_t>(time - start);
  return us >= diff ? us - diff : 0;
}

inline bool AdbInterfacePlatform::adb_delay_us(uint32_t delay) 
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
