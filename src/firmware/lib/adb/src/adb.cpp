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
#include "math.h"

#ifdef ADBUINO
#include <stdio.h>
#elif QUOKKADB
#include <time.h>
#include "rp2040_serial.h"
#include "printf/printf.h"
using rp2040_serial::Serial;
#endif



uint8_t mouse_addr = 0x03;
uint8_t kbd_addr = 0x02;
uint8_t mouse_handler_id = 0x01;
uint8_t kbd_handler_id = 0x02;
uint8_t mousepending = 0;
uint8_t kbdpending = 0;
uint8_t kbdskip = 0;
uint16_t kbdprev0 = 0;
uint16_t mousereg0 = 0;
uint16_t kbdreg0 = 0;
uint8_t kbdsrq = 0;
uint8_t mousesrq = 0;
uint8_t modifierkeys = 0xFF;
uint32_t kbskiptimer = 0;
bool adb_collision = false; 
bool collision_detection = false;
bool mouse_skip_next_listen_reg3 = false;
bool kbd_skip_next_listen_reg3 = false;


extern bool global_debug;

// The original data_lo code would just set the bit as an output
// That works for a host, since the host is doing the pullup on the ADB line,
// but for a device, it won't reliably pull the line low.  We need to actually
// set it.


inline bool AdbInterface::place_bit0(void)
{
  data_lo();
  adb_delay_us(65);
  data_hi();
  return adb_delay_us(35);
}
inline bool AdbInterface::place_bit1(void)
{
  data_lo();
  adb_delay_us(35);
  data_hi();
  return adb_delay_us(65);
}
inline bool AdbInterface::send_byte(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
    {
      if(!place_bit1()) return false;
    }
    else
    {
      if(!place_bit0()) return false;
    }
  }
  return true;
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


void AdbInterface::ProcessCommand(uint8_t cmd)
{

  uint16_t mousereg3, kbdreg3;

  // see if it is addressed to us
  if (((cmd >> 4) & 0x0F) == mouse_addr)
  {
    switch (cmd & 0x0F)
    {
    case 0x1:
      Serial.println("MOUSE: Got FLUSH request");
      break;
    case 0x8:
      Serial.println("MOUSE: Got LISTEN request for register 0");
      break;
    case 0x9:
      Serial.println("MOUSE: Got LISTEN request for register 1");
      break;
    case 0xA:
      Serial.println("MOUSE: Got LISTEN request for register 2");
      break;
    case 0xB:
      Serial.println("MOUSE: Got LISTEN request for register 3");
      break;
    case 0xC: // talk register 0
      if (global_debug) 

      if (mousepending)
      {
        DetectCollision();
        if (Send16bitRegister(mousereg0))
        {
          ResetCollision();
          mousepending = 0;
          mousesrq = 0;
        }
        else{
          ResetCollision();          
          mousesrq = 1;
          if (global_debug) 
          {
            Serial.println("MOUSE: Collision detected on sending register 0 on TALK request");
          }  
        }

        break;
      }
      break;
    case 0xD: // talk register 1
      Serial.println("MOUSE: Got TALK request for register 1");
      break;
    case 0xE: // talk register 2
      Serial.println("MOUSE Got TALK request for register 2");
      break;
    case 0xF: // talk register 3
      if (global_debug) 
      {
        Serial.println("MOUSE: Got TALK request for register 3");
      }
      // sets device address
      mousereg3 = GetAdbRegister3Mouse();
      DetectCollision();
      if( Send16bitRegister(mousereg3))
      {
        ResetCollision();
      }
      else
      {
        ResetCollision();
        mouse_skip_next_listen_reg3 = true;
          if (global_debug) 
          {
            Serial.println("MOUSE: Collision detected on sending register 3 on TALK request");
          }  
      }
      break;
    default:
      printf("MOUSE: Unknown cmd: %02X\n", cmd);
      break;
    }
  }
  else
  {
    if (mousepending)
      mousesrq = 1;
  }

  if (((cmd >> 4) & 0x0F) == kbd_addr)
  {
    uint8_t adbleds;
    uint16_t kbdreg2;
    switch (cmd & 0x0F)
    {
    case 0x1:
      Serial.println("KBD: Got FLUSH request");
      break;
    case 0x8:
      Serial.println("KBD: Got LISTEN request for register 0");
      break;
    case 0x9:
      Serial.println("KBD: Got LISTEN request for register 1");
      break;
    case 0xA:
      Serial.println("KBD: Got LISTEN request for register 2");
      break;
    case 0xB:
      Serial.println("KBD: Got LISTEN request for register 3");
      break;
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
        DetectCollision();
        if (Send16bitRegister(kbdreg0))
        {
          ResetCollision();
          kbdsrq = 0;
          kbdpending = 0;

        }
        else
        {
          ResetCollision();
          kbdsrq = 1;
          if (global_debug)
          {
            Serial.println("KBD: Collision detected on sending register 0 on TALK request");
          }

        }
      }
      break;
    case 0xD: // talk register 1
      Serial.println("KBD: Got TALK request for register 1");
      break;
    case 0xE: // talk register 2
      if (global_debug)
        Serial.println("KBD Got TALK request for register 2");
  
      adbleds = 0xFF; // Fine for normal keyboard (not extended)
      // if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
      // if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
      // if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;
      kbdreg2 = modifierkeys << 8;
      kbdreg2 |= adbleds;
      DetectCollision();
      if (Send16bitRegister(kbdreg2))
      {
        ResetCollision();
      }
      else
      {
        ResetCollision();
        if (global_debug)
        {
          Serial.println("KBD: Collision detected on sending register 2 on TALK request");
        }
      }
      
      break;
    case 0xF: // talk register 3
      Serial.println("KBD: Got TALK request for register 3");
      // sets device address
      kbdreg3 = GetAdbRegister3Keyboard();
      DetectCollision();
      if (Send16bitRegister(kbdreg3))
      {
          ResetCollision();
      }
      else
      {
        ResetCollision();
        kbd_skip_next_listen_reg3 = true;
        if (global_debug)
        {
          Serial.println("KBD: Collision detected on sending register 3 on TALK request");
        }        
      }
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
  // "ADB - The Untold Story: Space Aliens Ate My Mouse"
  // specifies that a random value should be returned as the address for register 3
kbdreg3 |= ((0x0F & rand()) << 8); 
  // 7-0       Device Handler ID
  kbdreg3 |= kbd_handler_id;
  
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
  // "ADB - The Untold Story: Space Aliens Ate My Mouse"
  // specifies that a random value should be returned as the address for register 3
  mousereg3 |= ((0x0F & rand()) << 8); 
  // 7-0       Device Handler ID
  mousereg3 |= mouse_handler_id;

  return mousereg3;
}