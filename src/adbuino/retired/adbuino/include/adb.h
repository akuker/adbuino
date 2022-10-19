//----------------------------------------------------------------------------
//
//  ADBuino ADB keyboard and mouse adapter
//	   Copyright (C) 2021-2022 bbraun
//	   Copyright (C) 2021-2022 akuker
//
//  This file is part of ADBuino.
//
//  ADBuino is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
// any later version.
//
//  ADBuino is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with ADBuino. If not, see <https://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include "Arduino.h"


class AdbInterface {
  public:
    void Init();
    //void ReadAdbCommand();
    uint8_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(uint8_t cmd);

    protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();

  private:
    const uint8_t m_adb_gpio = 3;
    const uint8_t m_pwr_gpio = 2;

    // These need to be looked upon the arduino schematic to figure out
    // which bank and databit the ADB pin is connected to
    #define ADB_PORT      PORTD
    #define ADB_PIN       PIND
    #define ADB_DDR       DDRD
    #define ADB_DATA_BIT  3


    void data_lo();
    void data_hi();
    uint8_t data_in();
    void adb_pin_out();
    void adb_pin_in();

    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    void place_bit0(void);
    void place_bit1(void);
    void send_byte(uint8_t data);
};
