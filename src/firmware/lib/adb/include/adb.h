
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

extern bool adb_collision;
extern bool collision_detection;

class AdbInterface : public AdbInterfacePlatform {
  public:
    //void ReadAdbCommand();
    uint8_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(uint8_t cmd);

  protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();
    bool Send16bitRegister(uint16_t reg16);
    void DetectCollision(void);
    void ResetCollision(void);

  private:
    bool place_bit0(void);
    bool place_bit1(void);
    bool send_byte(uint8_t data);
};


inline bool AdbInterface::Send16bitRegister(uint16_t reg16)
{
          // stop to start time / interframe delay
        if (!adb_delay_us(180)) return false;  
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

inline void AdbInterface::DetectCollision(void)
{
  collision_detection = true;
}

inline void AdbInterface::ResetCollision(void)
{
  collision_detection = false;
  adb_collision = false;
}