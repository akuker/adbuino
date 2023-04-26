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

#pragma once

#include "Arduino.h"
#include <stdint.h>

class AdbInterfacePlatform {
  public:
    void Init();

  protected:
    // These need to be looked upon the arduino schematic to figure out
    // which bank and databit the ADB pin is connected to
    #define ADB_PORT      PORTD
    #define ADB_PIN       PIND
    #define ADB_DDR       DDRD
    #define ADB_DATA_BIT  3
    #define ADB_DATA_ARDUINO_PIN_NUM 3
    
    void data_lo();
    void data_hi();
    void adb_pin_out();
    void adb_pin_in();
    uint8_t data_in();

    
    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    
    bool adb_delay_us(double delay);

    void adb_irq_init(void);
    void adb_irq_disable(void);

    void adb_set_leds(uint16_t reg2){};

};


inline void AdbInterfacePlatform::data_lo()
{
  (ADB_DDR |= (1 << ADB_DATA_BIT));
  (ADB_PORT &= ~(1 << ADB_DATA_BIT));
}

inline void AdbInterfacePlatform::data_hi()
{
  (ADB_DDR &= ~(1 << ADB_DATA_BIT));
}
inline uint8_t AdbInterfacePlatform::data_in()
{
  return (ADB_PIN & (1 << ADB_DATA_BIT));
}

inline void AdbInterfacePlatform::adb_pin_out()
{
  (ADB_DDR |= (1 << ADB_DATA_BIT));
}
inline void AdbInterfacePlatform::adb_pin_in()
{
  (ADB_DDR &= ~(1 << ADB_DATA_BIT));
}

inline uint16_t AdbInterfacePlatform::wait_data_lo(uint32_t us)
{
  do
  {
    if (!data_in())
      break;
    _delay_us(1 - (6 * 1000000.0 / F_CPU));
  } while (--us);
  return us;
}

inline uint16_t AdbInterfacePlatform::wait_data_hi(uint32_t us)
{
  do
  {
    if (data_in())
      break;
    _delay_us(1 - (6 * 1000000.0 / F_CPU));
  } while (--us);
  return us;
}

inline bool AdbInterfacePlatform::adb_delay_us(double delay)
{
    // for this _delay_us to compile "__DELAY_BACKWARD_COMPATIBLE__" 
    // must be defined in the platformio.ini file. Otherwise
    // it can only be delayed with compile time computations
    _delay_us(delay);
    return true; 
}
