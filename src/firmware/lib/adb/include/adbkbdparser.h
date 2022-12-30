//----------------------------------------------------------------------------
//
//  ADBuino & QuokkADB ADB keyboard and mouse adapter
//     Copyright (C) 2007 Peter H Anderson
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the ADBuino & QuokkADB and projects
//
//  This file is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
// any later version.
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

#include "usbkbdparser.h"
#ifdef ADBUINO
#include <hidboot.h>
#elif QUOKKADB
#include "keyboardrptparser.h"
#endif

#include "scqueue.h"

#define ADB_POWER_KEYCODE 0x7f

using simple_circular_queue::SCQueue;

extern uint8_t usb_keycode_to_adb_code(uint8_t usb_code);


class ADBKbdRptParser : public KbdRptParser
{
public:
    ADBKbdRptParser();

    uint16_t GetAdbRegister0();
    uint16_t GetAdbRegister2();

};
