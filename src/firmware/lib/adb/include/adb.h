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
//----------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <adb_platform.h>

#ifndef ADB_START_BIT_DELAY
#define ADB_START_BIT_DELAY 100000
#endif

#define KBD_DEFAULT_ADDR 0x02
#define KBD_DEFAULT_HANDLER_ID 0x02
#define MOUSE_DEFAULT_ADDR 0x03
#define MOUSE_DEFAULT_HANDLER_ID 0x01
#define KDB_EXTENDED_HANDLER_ID 0x3

extern bool adb_collision;
extern bool collision_detection;

class AdbInterface : public AdbInterfacePlatform {
  public:
    //void ReadAdbCommand();
    int16_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(int16_t cmd);
    void Reset(void);

  protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();
    bool Send16bitRegister(uint16_t reg16);
    // returns less than zero on a failed receive
    int32_t Receive16bitRegister(void);
    void DetectCollision(void);
    void ResetCollision(void);

  private:
    bool place_bit0(void);
    bool place_bit1(void);
    bool send_byte(uint8_t data);
};


inline bool AdbInterface::Send16bitRegister(uint16_t reg16)
{
    // stop to start time / interframe delay - min time 140us, max time 260. 
    // adding randomness as suggested by Apple Guide the Mac. family  Hardware 2nd edition
    // pg 324.  Random time delay will give a max stop to start time of 240us
   uint32_t extra_delay = (rand() % 101);
   if (!adb_delay_us(140 + extra_delay)) return false; 
  adb_pin_out();
  // start bit
  if (!place_bit1()) return false; 
  if (!send_byte((reg16 >> 8) & 0x00FF))  return false;
  if (!send_byte(reg16 & 0x00FF))  return false;
  // stop bit
  if (!place_bit0()) return false;
  adb_pin_in();
  return true;
}


inline int32_t AdbInterface::Receive16bitRegister(void)
{        
  int32_t data = 0;
  uint16_t hi_time;
  uint16_t low_time;

  hi_time = wait_data_lo(1000);
  // start-stop time is officially > 140 and < 260
  if (!hi_time || hi_time < 130 || hi_time > 270 )
  {
    return -1;  
  }

  // start bit 
  low_time = wait_data_hi(130);
 if (!low_time || low_time > 45 || low_time < 25)
  {
    return -2;
  }
  

  hi_time = wait_data_lo(130);
  if (!hi_time || hi_time > 75 || hi_time < 55)
  {
    return -3;
  }

  for (uint8_t bits = 0; bits < 16; bits++)
  {
    uint8_t lo = wait_data_hi(130);
    if (!lo)
    {
      goto out;
    }
    uint8_t hi = wait_data_lo(100);
    if (!hi)
    {
      goto out;
    }
    if (120 < lo + hi )
    {
      goto out;
    }

    data <<= 1;
    if (lo < 40)
    {
      data |= 1;
    }
  }

  // stop bit
  low_time = wait_data_hi(130);
  if (!low_time || low_time > 70)
  {  
    return -4;
  }
  
  return data;
out:
  return -5;
}


inline void AdbInterface::DetectCollision(void)
{
  adb_irq_init();
  collision_detection = true;
}

inline void AdbInterface::ResetCollision(void)
{
  adb_irq_disable();
  collision_detection = false;
  adb_collision = false;
}