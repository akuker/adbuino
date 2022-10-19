//---------------------------------------------------------------------------
//
//	ADBuino & QuokkaADB ADB keyboard and mouse adapter
//
//     Copyright 2011 Jun WAKO <wakojun@gmail.com>
//     Copyright 2013 Shay Green <gblargg@gmail.com>
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2020 difegue
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
//  Portions of this code were originally released under a Modified BSD 
//  License. See LICENSE in the root of this repository for more info.
//
//----------------------------------------------------------------------------

#include "adb.h"
#include "bithacks.h"

#ifdef ADBUINO
#include <stdio.h>
#elif QUOKKADB
#include <time.h>
#include "rp2040_serial.h"
#include "printf/printf.h"
using rp2040_serial::Serial;
#endif


extern uint8_t mousepending;
extern uint8_t kbdpending;
extern uint8_t kbdskip;
extern uint16_t kbdprev0;
extern uint16_t mousereg0;
extern uint16_t kbdreg0;
extern uint8_t kbdsrq;
extern uint8_t mousesrq;
extern uint8_t modifierkeys;
extern uint32_t kbskiptimer;
extern bool global_debug;
// The original data_lo code would just set the bit as an output
// That works for a host, since the host is doing the pullup on the ADB line,
// but for a device, it won't reliably pull the line low.  We need to actually
// set it.


inline void AdbInterface::place_bit0(void)
{
  data_lo();
  adb_delay_us(65);
  data_hi();
  adb_delay_us(35);
}
inline void AdbInterface::place_bit1(void)
{
  data_lo();
  adb_delay_us(35);
  data_hi();
  adb_delay_us(65);
}
inline void AdbInterface::send_byte(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
      place_bit1();
    else
      place_bit0();
  }
}
uint8_t AdbInterface::ReceiveCommand(uint8_t srq)
{
  uint8_t bits;
  uint16_t data = 0;
  // Serial.println("Checking for command");

  // find attention & start bit
  if (!wait_data_lo(ADB_START_BIT_DELAY))
    return 0;
  uint16_t lowtime = wait_data_hi(1000);
  if (!lowtime || lowtime > 500)
  {
    return 0;
  }
  wait_data_lo(100);
  for (bits = 0; bits < 8; bits++)
  {
    uint8_t lo = wait_data_hi(130);
    if (!lo)
    {
      goto out;
    }
    uint8_t hi = wait_data_lo(lo);
    if (!hi)
    {
      goto out;
    }
    hi = lo - hi;
    lo = 130 - lo;

    data <<= 1;
    if (lo < hi)
    {
      data |= 1;
    }
  }

  if (srq)
  {
    data_lo();
    adb_delay_us(250);
    data_hi();
  }
  else
  {
    // Stop bit normal low time is 70uS + can have an SRQ time of 300uS
    wait_data_hi(400);
  }
  return data;
out:
  return 0;
}

bool first_mouse = true;
bool first_keybd = true;
void AdbInterface::ProcessCommand(uint8_t cmd)
{

  uint16_t mousereg3, kbdreg3;

  // see if it is addressed to us
  if (((cmd >> 4) & 0x0F) == 3)
  {
    switch (cmd & 0x0F)
    {
    case 0xC: // talk register 0
      if (mousepending)
      {
        adb_delay_us(180); // stop to start time / interframe delay
        adb_pin_out();
        place_bit1(); // start bit
        send_byte((mousereg0 >> 8) & 0x00FF);
        send_byte(mousereg0 & 0x00FF);
        place_bit0(); // stop bit
        adb_pin_in();
        mousepending = 0;
        mousesrq = 0;
      }
      break;
    case 0xD: // talk register 1
      Serial.println("MOUSE: Got request for register 1");
      break;
    case 0xE: // talk register 2
      Serial.println("MOUSE Got request for register 2");
      break;
    case 0xF: // talk register 3
      Serial.println("MOUSE: Got request for register 3");
      // // sets device address
      // mousereg3 = GetAdbRegister3Mouse();
      // adb_delay_us(180); // stop to start time / interframe delay
      // //   ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
      // adb_pin_out();
      // place_bit1(); // start bit
      // send_byte((mousereg3 >> 8) & 0x00FF);
      // send_byte(mousereg3 & 0x00FF);
      // place_bit0(); // stop bit
      // //   ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
      // adb_pin_in();
      break;
    default:
      printf("MOUSE: Unknown cmd: %02X", cmd);
      break;
    }
  }
  else
  {
    if (mousepending)
      mousesrq = 1;
  }

  if (((cmd >> 4) & 0x0F) == 2)
  {
    uint8_t adbleds;
    switch (cmd & 0x0F)
    {
    case 0xC: // talk register 0
      if (kbdpending)
      {

        if (kbdskip)
        {
          kbdskip = 0;
          // Serial.println("Skipping invalid 255 signal and sending keyup instead");

          // Send a 'key released' code to avoid ADB sticking to the previous key
          kbdprev0 |= 0x80;
          kbdreg0 = (kbdprev0 << 8) | 0xFF;

          // Save timestamp
          kbskiptimer = millis();
        }
        else if (millis() - kbskiptimer < 90)
        {
          // Check timestamp and don't process the key event if it came right after a 255
          // This is meant to avoid a glitch where releasing a key sends 255->keydown instead
          printf("Too little time since bugged keyup, skipping this keydown event");
          kbdpending = 0;
          break;
        }
        kbdsrq = 0;
        adb_delay_us(180); // stop to start time / interframe delay
        adb_pin_out();
        place_bit1(); // start bit
        send_byte((kbdreg0 >> 8) & 0x00FF);
        send_byte(kbdreg0 & 0x00FF);
        place_bit0(); // stop bit
        adb_pin_in();
        kbdpending = 0;
      }
      break;
    case 0xD: // talk register 1
      Serial.println("KBD: Got request for register 1");
      break;
    case 0xE: // talk register 2
      Serial.println("KBD Got request for register 2");
      adbleds = 0xFF; // we should get the actual value
      // if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
      // if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
      // if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;

      adb_delay_us(180); // stop to start time / interframe delay
      adb_pin_out();
      place_bit1(); // start bit
      send_byte(modifierkeys);
      send_byte(adbleds);
      place_bit0(); // stop bit
      adb_pin_in();
      break;
    case 0xF: // talk register 3
      Serial.println("KBD: Got request for register 3");
      // sets device address
      kbdreg3 = GetAdbRegister3Keyboard();
      adb_delay_us(180); // stop to start time / interframe delay
      adb_pin_out();
      place_bit1(); // start bit
      send_byte((kbdreg3 >> 8) & 0x00FF);
      send_byte(kbdreg3 & 0x00FF);
      place_bit0(); // stop bit
      adb_pin_in();
      break;
    default:
      printf("KBD: Unknown cmd: %02X\n", cmd);
      break;
    }
  }
  else
  {
    if (kbdpending)
      kbdsrq = 1;
  }
}

uint16_t AdbInterface::GetAdbRegister3Keyboard()
{
  uint16_t kbdreg3 = 0;

  // Bit 15 Reserved; must be 0
  B_UNSET(kbdreg3, 15);
  // 14      Exceptional event, device specific; always 1 if not used
  B_SET(kbdreg3, 14);
  // 13      Service Request enable; 1 = enabled
  B_UNSET(kbdreg3, 13);
  // 12      Reserved; must be 0
  B_UNSET(kbdreg3, 12);
  // 11-8      Device address
  kbdreg3 |= (0x02 << 8);
  // 7-0       Device Handler ID

  return kbdreg3;
}
uint16_t AdbInterface::GetAdbRegister3Mouse()
{
  uint16_t mousereg3 = 0;

  // Bit 15 Reserved; must be 0
  B_UNSET(mousereg3, 15);
  // 14      Exceptional event, device specific; always 1 if not used
  B_SET(mousereg3, 14);
  // 13      Service Request enable; 1 = enabled
  B_UNSET(mousereg3, 13);
  // 12      Reserved; must be 0
  B_UNSET(mousereg3, 12);
  // 11-8      Device address
  mousereg3 |= (0x03 << 8);
  // 7-0       Device Handler ID

  return mousereg3;
}