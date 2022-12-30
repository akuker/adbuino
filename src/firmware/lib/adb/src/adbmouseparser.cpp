//---------------------------------------------------------------------------
//
//	ADBuino & QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the  ADBuino and the QuokkaADB projects.
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
//  with file. If not, see <https://www.gnu.org/licenses/>.
//
//----------------------------------------------------------------------------


#include "adbmouseparser.h"
#ifdef QUOKKADB
#include "quokkadb_gpio.h"
#include "rp2040_serial.h"
using rp2040_serial::Serial;
#endif

extern bool global_debug;

ADBMouseRptParser::ADBMouseRptParser(ADBKbdRptParser &kbd_parser)
{
    m_keyboard = &kbd_parser;
}

uint32_t ADBMouseRptParser::EightBitToSevenBitSigned(int8_t value)
{
    // Dividing the value by 2 has 2 benefits:
    //   - Easy way to convert from 8 bit to 7 bit number
    //   - It "softens" the mouse movement. Modern optical
    //     mice seem a little jumpy.
    return (uint32_t)((value/2) & 0x7F);
}

uint16_t ADBMouseRptParser::GetAdbRegister0()
{
    uint16_t reg_value = 0;
    // Bit 15 = Left Button Status; 0=down
    if (!IsLeftButtonPressed())
    {
        reg_value |= (1 << 15);
    }
    // Bit 7 = Right Button Status - introduced in System 8
    if (!IsRightButtonPressed())
    {
        reg_value |= (1 << 7);
    }

    // // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= (EightBitToSevenBitSigned(GetDeltaY()) << 8);

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= (EightBitToSevenBitSigned(GetDeltaX()) << 0);

    ResetMouseMovement();

    return reg_value;
}
