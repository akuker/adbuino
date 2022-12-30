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

uint8_t mouse_addr = MOUSE_DEFAULT_ADDR;
uint8_t kbd_addr = KBD_DEFAULT_ADDR;
uint8_t mouse_handler_id = MOUSE_DEFAULT_HANDLER_ID;
uint8_t kbd_handler_id = KBD_DEFAULT_HANDLER_ID;
uint8_t mousepending = 0;
uint8_t kbdpending = 0;
uint8_t kbdskip = 0;
uint16_t kbdprev0 = 0;
uint16_t mousereg0 = 0;
uint16_t kbdreg0 = 0;
uint16_t kbdreg2 = 0xFFFF;
uint8_t kbdsrq = 0;
uint8_t mousesrq = 0;
uint16_t modifierkeys = 0xFFFF;
uint32_t kbskiptimer = 0;
bool adb_reset = false;
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
int16_t AdbInterface::ReceiveCommand(uint8_t srq)
{
  uint8_t bits; 
  uint16_t lo, hi;
  int16_t data = 0;
  
  // find attention & start bit
  hi = wait_data_lo(ADB_START_BIT_DELAY); 
  if (!hi)
    return -1;
  do 
  {
    lo = wait_data_hi(4000);
    if (!lo || lo > 820 || lo < 780)
    {
      if (lo > 2950) 
      {
        adb_reset = true;
        if (global_debug)
        {
          printf("ALL: Global reset detected, wait time was %dus\n", lo);
        }
        return -100;
      }
      else {
        if (global_debug)
        {
          printf("ALL: Error in attention low time,  wait time was %dus\n", lo);

        }
      }
      return -1;

    }
    else
    {
      // Attention was ~800us like it should be
      break;
    }
  }
  while(true);

  hi = wait_data_lo(100);
  if (!hi && hi > 70 && hi < 40)
  {
    if (global_debug)
    {
      printf("Start bit not found, wait time was %dus\n", hi);
    }
    return -3;
  }
  
  for (bits = 0; bits < 8; bits++)
  {
    lo = wait_data_hi(130);
    if (!lo)
    {
      goto out;
    }
    hi = wait_data_lo(100);
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
  if (global_debug)
  {
    printf("ALL: Error reading CMD bits, low time %hhu, high time %hhu at bit %hhu\n", lo, hi, bits);
  }
  return -4;
}


void AdbInterface::ProcessCommand(int16_t cmd)
{
  uint8_t  listen_addr, listen_handler_id;
  uint16_t mousereg3, kbdreg3;
  int32_t listen_register;

  if (cmd < 0)
  {
    // -1 is waiting for a signal
    // -100 is a 3ms ADB reset
    if (-1 == cmd || -100 == cmd)
    {
      if (cmd == -100) 
      {
        if (global_debug)
        {
          Serial.println("ALL: Global 3ms reset signal");
        }
      }
      return;
    }

    if (global_debug)
    {
      printf("ALL: CMD code error, cmd: %d\n", cmd);
    }
    return;
  }

  if ((0x0F & cmd) == 0x00) 
  {
    adb_reset = true;
    if (global_debug)
    {
      Serial.println("ALL: Cmd for reset all devices");
    }
    return;
  }
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
      listen_register = Receive16bitRegister();
      if (global_debug)
      {
        printf("MOUSE: Got LISTEN request for register 3 at address 0x%hhX\n", mouse_addr);
      }

      if (listen_register >= 0)
      {
        listen_addr = (listen_register >> 8) & 0x0F;
        listen_handler_id = listen_register & 0xFF;
        if (global_debug)
        {
          printf("MOUSE: Listen Register 3 value is 0x%lX\n", listen_register);
        }
        // self-test
        if (0xFF == listen_handler_id)
        {
          break;
        }
        // Change of address 
        if (0xFE == listen_handler_id )
        {

            if (mouse_skip_next_listen_reg3)
            {
              mouse_skip_next_listen_reg3 = false;
              if (global_debug)
              {
                printf("MOUSE: TALK reg 3 had a collision at 0x%hhX\n", mouse_addr);
              }
              break;
            }
            mouse_addr = listen_addr;
            if (global_debug)
            {
              printf("MOUSE: address change to 0x%hhX\n", mouse_addr);
            }
        }
        else
        {
          
          // Don't change address for mouse, handler id values can be 1, 2, or 4.
          //   1 - standard mouse 
          //   2 - standard mouse with extra sensitivity
          //   4 - extended mouse 
          /*  Don't change mouse type from handler id 1 (default)
          if (listen_handler_id == 1 || listen_handler_id == 2)
          {
            mouse_handler_id  = listen_handler_id;
          }
          */
          if (global_debug)
          {
            printf("MOUSE: LSTN Reg3 val is 0x%lX@0x%hhX\n", listen_register, mouse_addr);
            printf("MOUSE: handler id change to  0x%hhX\n", mouse_handler_id);
          }
        }
      } 
      else
      {
        if (global_debug)
        {
          printf("MOUSE: Listen Register 3 errored with code %ld\n", listen_register);
        }
      }
      break;
    case 0xC: // talk register 0

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
            printf("MOUSE: Collision on sending register 0 on TALK request at address 0x%hhX\n", mouse_addr);
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
          printf("MOUSE: Collision TALK register 3 at 0x%hhX\n", mouse_addr);
        }
      }
      if (global_debug)
      {
        printf("MOUSE: Got TALK request for register 3 at address 0x%hhX\n", mouse_addr);
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
    switch (cmd & 0x0F)
    {
    case 0x1:
      if (global_debug)
      {
        Serial.println("KBD: Got FLUSH request");
      }
      break;
    case 0x8:
      Serial.println("KBD: Got LISTEN request for register 0");
      
      break;
    case 0x9:
      Serial.println("KBD: Got LISTEN request for register 1");
      break;
    case 0xA:
      if (global_debug)
      {
        Serial.println("KBD: Got LISTEN request for register 2");
      }
      listen_register = Receive16bitRegister();
      
      if (KDB_EXTENDED_HANDLER_ID == kbd_handler_id)
      {
        adb_set_leds(listen_register);
      }
      break;
    case 0xB:
      listen_register = Receive16bitRegister();
      if (global_debug)
      {
        printf("KBD: Got LISTEN request for register 3 at address 0x%hhX\n", kbd_addr);
      }
      if (listen_register >= 0)
      {
        listen_addr = (listen_register >> 8) & 0x0F;
        listen_handler_id = listen_register & 0xFF;
        if (global_debug)             
        {
          printf("KBD: Listen Register 3 value is 0x%lX\n", listen_register);
        }
        // self-test
        if (0xFF == listen_handler_id)
        {
          break;
        }
        // Change of address 
        if (0xFE == listen_handler_id)
        {

            if (kbd_skip_next_listen_reg3)
            {
              kbd_skip_next_listen_reg3 = false;
              if (global_debug)
              {
                printf("KDB: had a collision reg 3 at 0x%hhX\n", kbd_addr);
              }
              break;
            }
            kbd_addr = listen_addr;
            if (global_debug)
            {
              printf("KBD: address change to 0x%hhX\n", kbd_addr);
            }

        }
        else
        {
          if (KDB_EXTENDED_HANDLER_ID == listen_handler_id || KBD_DEFAULT_HANDLER_ID == listen_handler_id)
          { 
            kbd_handler_id = listen_handler_id;
          }
          if (global_debug)
          {
            printf("KBD: address change to 0x%hhX, handler id change to 0x%hhX\n", kbd_addr, kbd_handler_id);
          }

        }
      }  
      else
      {
        if (global_debug)
        {
          printf("KBD: Listen Register 3 errored with code %ld\n", listen_register);
        }
      }
      
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
      {
        Serial.println("KBD Got TALK request for register 2");
      }
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
      if (global_debug) 
      { 
        Serial.println("KBD: Got TALK request for register 3");
      }
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
          printf("KBD: Collision TALK register 3 at 0x%hhX\n", kbd_addr);             
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
  // using random address 
  uint8_t random_address = rand() & 0xF;
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
  kbdreg3 |=  random_address << 8;
  // 7-0       Device Handler ID
  kbdreg3 |= kbd_handler_id;
  
  return kbdreg3;
}
uint16_t AdbInterface::GetAdbRegister3Mouse()
{
  uint16_t mousereg3 = 0;
  // using random address in 0x8 to 0xE addresses
  uint8_t random_address =  rand() & 0xF;
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
  mousereg3 |= random_address << 8;  
  // 7-0       Device Handler ID
  mousereg3 |= mouse_handler_id;

  return mousereg3;
}

void AdbInterface::Reset(void)
{
  mouse_addr = MOUSE_DEFAULT_ADDR;
  kbd_addr = KBD_DEFAULT_ADDR;
  mouse_handler_id = MOUSE_DEFAULT_HANDLER_ID;
  kbd_handler_id = KBD_DEFAULT_HANDLER_ID;
  kbdreg2 = 0xFFFF;
}
