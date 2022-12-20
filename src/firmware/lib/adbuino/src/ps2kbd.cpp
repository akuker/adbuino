//---------------------------------------------------------------------------
//
//	ADBuino ADB keyboard and mouse adapter
//
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2020 difegue
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
//---------------------------------------------------------------------------

#include <stdint.h>

#define kModCmd 1
#define kModOpt 2
#define kModShift 4
#define kModControl 8
#define kModReset 16
#define kModCaps 32
#define kModDelete 64
extern uint8_t mousepending;
extern uint8_t kbdpending;
extern uint8_t kbdskip;
extern uint16_t kbdprev0;
extern uint16_t mousereg0;
extern uint16_t kbdreg0;
extern uint8_t kbdsrq;
extern uint8_t mousesrq;
extern uint16_t modifierkeys;
extern uint32_t kbskiptimer;


uint8_t PS2ToADBScancode(uint8_t code, uint8_t keyup, uint8_t extended)
{
  uint8_t maccode = 0xFF;
  if (!extended)
  {
    switch (code)
    {
    case 0x1C:
      maccode = 0x00;
      break; // A
    case 0x32:
      maccode = 0x0B;
      break; // B
    case 0x21:
      maccode = 0x08;
      break; // C
    case 0x23:
      maccode = 0x02;
      break; // D
    case 0x24:
      maccode = 0x0E;
      break; // E
    case 0x2B:
      maccode = 0x03;
      break; // F
    case 0x34:
      maccode = 0x05;
      break; // G
    case 0x33:
      maccode = 0x04;
      break; // H
    case 0x43:
      maccode = 0x22;
      break; // I
    case 0x3B:
      maccode = 0x26;
      break; // J
    case 0x42:
      maccode = 0x28;
      break; // K
    case 0x4B:
      maccode = 0x25;
      break; // L
    case 0x3A:
      maccode = 0x2E;
      break; // M
    case 0x31:
      maccode = 0x2D;
      break; // N
    case 0x44:
      maccode = 0x1F;
      break; // O
    case 0x4D:
      maccode = 0x23;
      break; // P
    case 0x15:
      maccode = 0x0C;
      break; // Q
    case 0x2D:
      maccode = 0x0F;
      break; // R
    case 0x1B:
      maccode = 0x01;
      break; // S
    case 0x2C:
      maccode = 0x11;
      break; // T
    case 0x3C:
      maccode = 0x20;
      break; // U
    case 0x2A:
      maccode = 0x09;
      break; // V
    case 0x1D:
      maccode = 0x0D;
      break; // W
    case 0x22:
      maccode = 0x07;
      break; // X
    case 0x35:
      maccode = 0x10;
      break; // Y
    case 0x1A:
      maccode = 0x06;
      break; // Z
    case 0x45:
      maccode = 0x1D;
      break; // 0
    case 0x16:
      maccode = 0x12;
      break; // 1
    case 0x1E:
      maccode = 0x13;
      break; // 2
    case 0x26:
      maccode = 0x14;
      break; // 3
    case 0x25:
      maccode = 0x15;
      break; // 4
    case 0x2E:
      maccode = 0x17;
      break; // 5
    case 0x36:
      maccode = 0x16;
      break; // 6
    case 0x3D:
      maccode = 0x1A;
      break; // 7
    case 0x3E:
      maccode = 0x1C;
      break; // 8
    case 0x46:
      maccode = 0x19;
      break; // 9
    case 0x0E:
      maccode = 0x32;
      break; // `
    case 0x4E:
      maccode = 0x1B;
      break; // -
    case 0x55:
      maccode = 0x18;
      break; // =
    case 0x5D:
      maccode = 0x2A;
      break; // backslash
    case 0x66:
      maccode = 0x33;
      break; // bkspc
    case 0x29:
      maccode = 0x31;
      break; // space
    case 0x0D:
      maccode = 0x30;
      break; // tab
    case 0x58:
      maccode = 0x39;
      break; // caps
    case 0x12:
      maccode = 0x38;
      break; // left shift
    case 0x59:
      maccode = 0x7B;
      break; // right shift
    case 0x11:
      maccode = 0x3A;
      break; // left alt = opt
    case 0x5A:
      maccode = 0x24;
      break; // enter
    case 0x41:
      maccode = 0x2B;
      break; // ,
    case 0x49:
      maccode = 0x2F;
      break; // .
    case 0x4A:
      maccode = 0x2C;
      break; // /
    case 0x14:
      maccode = 0x36;
      break; // L control
    case 0x76:
      maccode = 0x35;
      break; // ESC
    case 0x52:
      maccode = 0x27;
      break; // '
    case 0x4C:
      maccode = 0x29;
      break; // ;
    case 0x7C:
      maccode = 0x43;
      break; // keypad *
    case 0x7B:
      maccode = 0x4E;
      break; // keypad -
    case 0x79:
      maccode = 0x45;
      break; // keypad +
    case 0x71:
      maccode = 0x41;
      break; // keypad .
    case 0x70:
      maccode = 0x52;
      break; // keypad 0
    case 0x69:
      maccode = 0x53;
      break; // keypad 1
    case 0x72:
      maccode = 0x54;
      break; // keypad 2
    case 0x7A:
      maccode = 0x55;
      break; // keypad 3
    case 0x6B:
      maccode = 0x56;
      break; // keypad 4
    case 0x73:
      maccode = 0x57;
      break; // keypad 5
    case 0x74:
      maccode = 0x58;
      break; // keypad 6
    case 0x6C:
      maccode = 0x59;
      break; // keypad 7
    case 0x75:
      maccode = 0x5B;
      break; // keypad 8
    case 0x7D:
      maccode = 0x5C;
      break; // keypad 9
    case 0x05:
      maccode = 0x7A;
      break; // F1
    case 0x06:
      maccode = 0x78;
      break; // F2
    case 0x04:
      maccode = 0x63;
      break; // F3
    case 0x0C:
      maccode = 0x76;
      break; // F4
    case 0x03:
      maccode = 0x60;
      break; // F5
    case 0x0B:
      maccode = 0x61;
      break; // F6
    case 0x83:
      maccode = 0x62;
      break; // F7
    case 0x0A:
      maccode = 0x64;
      break; // F8
    case 0x01:
      maccode = 0x65;
      break; // F9
    case 0x09:
      maccode = 0x6D;
      break; // F10
    case 0x78:
      maccode = 0x67;
      break; // F11
    case 0x07:
      maccode = 0x6F;
      break; // F12
    case 0x54:
      maccode = 0x21;
      break; // [
    case 0x5B:
      maccode = 0x1E;
      break; // ]
    default:
      break;
    }
  }
  else
  {
    switch (code)
    {
    case 0x1F:
      maccode = 0x37;
      break; // left windows key = cmd
    case 0x27:
      maccode = 0x37;
      break; // right windows key = cmd
    case 0x11:
      maccode = 0x7C;
      break; // right alt = cmd
    case 0x5A:
      maccode = 0x4C;
      break; // keypad enter
    case 0x4A:
      maccode = 0x4B;
      break; // keypad /
    case 0x75:
      maccode = 0x3E;
      break; // up -> keypad up
    case 0x6B:
      maccode = 0x3B;
      break; // left arrow
    case 0x72:
      maccode = 0x3D;
      break; // down arrow
    case 0x74:
      maccode = 0x3C;
      break; // right arrow
    case 0x14:
      maccode = 0x7D;
      break; // R control
    case 0x7A:
      maccode = 0x79;
      break; // PG DN
    case 0x7D:
      maccode = 0x74;
      break; // PG UP
    case 0x6C:
      maccode = 0x73;
      break; // Home
    case 0x69:
      maccode = 0x77;
      break; // End
    case 0x71:
      maccode = 0x75;
      break; // Del
    case 0x70:
      maccode = 0x72;
      break; // Insert/Help
    }
  }
#define MODKEYSET(y) keyup ? (modifierkeys |= (y)) : (modifierkeys &= ~(y))
  switch (maccode)
  {
  case 0x37:
    MODKEYSET(kModCmd);
    break;
  case 0x7C:
  case 0x3A:
    MODKEYSET(kModOpt);
    break;
  case 0x7B:
  case 0x38:
    MODKEYSET(kModShift);
    break;
  case 0x7D:
  case 0x36:
    MODKEYSET(kModControl);
    break;
  case 0x39:
    MODKEYSET(kModCaps);
    break;
  case 0x33:
    MODKEYSET(kModDelete);
    break;
  }
#if PWRBUTTON
  if ((!(modifierkeys & kModShift)) && (maccode == 0x35))
  {
    MODKEYSET(kModReset);
    if (keyup)
    {
      GPIO_SetBits(GPIOB, kADBPowerPin);
      USART2_SendString("Power key up\n\r");
      __disable_irq();
      adbkey[adbend] = 0xFF;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0xFF;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0xFF;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0x38;
      adbend = (adbend + 1) % kADBBuffSize;
      adb.senddata |= (1 << 2);
      __enable_irq();
    }
    else
    {
      GPIO_ResetBits(GPIOB, kADBPowerPin);
      USART2_SendString("Power key down\n\r");
      __disable_irq();
      adbkey[adbend] = 0x38 | 0x80;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0x7F;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0x7F;
      adbend = (adbend + 1) % kADBBuffSize;
      adbkey[adbend] = 0x7F;
      adbend = (adbend + 1) % kADBBuffSize;
      adb.senddata |= (1 << 2);
      __enable_irq();
    }
    return 0x7F;
  }
#endif
  if (keyup)
  {
    maccode |= 0x80;
  }

  if (maccode == 0xFF)
  {
    // 255 codes are invalid(bad keyboard?), set the skip flag
    kbdskip = 1;
  }
  else
  {
    // Save valid codes so we can fallback to them in case of 255
    kbdprev0 = maccode;
  }

  kbdreg0 = (maccode << 8) | 0xFF;
  kbdpending = 1;
  return maccode;
}
