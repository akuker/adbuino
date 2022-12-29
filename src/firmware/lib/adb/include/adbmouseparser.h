//----------------------------------------------------------------------------
//
//  ADBuino & QuokkADB ADB keyboard and mouse adapter
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2007 Peter H Anderson
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the ABDuino and the QuokkADB projects.
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
//  with this file. If not, see <https://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
#pragma once
#include "adbkbdparser.h"
#include "usbmouseparser.h"

class ADBMouseRptParser : public MouseRptParser
{
public:
    ADBMouseRptParser(ADBKbdRptParser& kbd_parser);
    uint16_t GetAdbRegister0();

protected:
    uint32_t EightBitToSevenBitSigned(int8_t value);

};
