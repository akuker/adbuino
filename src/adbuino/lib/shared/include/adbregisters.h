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

// Bit definitions from "Guide to the Macintosh Family Hardware", Table 8-10 and 8-11

#ifndef _ADBREGISTERS_H
#define _ADBREGISTERS_H

// Table 8-10 Register 0 in the Apple Extended Keyboard
//    Bit  = Meaning
//    15   = Key status for -first key; 0 = down
//    14-8 = Key code for first key; a 7-bit ASQI value
//    7    = Key status for second key; 0 = down
//    6-0  = Key code for second key; a 7-bit ASCII value
#define ADB_REG_0_KEY_1_STATUS_BIT 15
#define ADB_REG_0_KEY_1_KEY_CODE   8
#define ADB_REG_0_KEY_2_STATUS_BIT 7
#define ADB_REG_0_KEY_2_KEY_CODE   0
#define ADB_REG_0_NO_KEY 0xFF

// Table 8-11 Register 2 in the Apple Extended Keyboard
//  Bit   Meaning
//  15  = None (reserved)
//  14  = Delete
//  13  = Caps Lock
//  12  = Reset
//  11  = Control
//  10  = Shift
//  9   = Option
//  8   = Command
//  7   = Num Lock/Clear
//  6   = Scroll Lock
//  5-3 = None (reserved)
//  2   = LED 3 (Scroll Lock)
//  1   = LED 2 (Caps Lock)
//  0   = LED 1 (Num Lock)
#define ADB_REG_2_FLAG_DELETE 14
#define ADB_REG_2_FLAG_CAPS_LOCK 13
#define ADB_REG_2_FLAG_RESET 12
#define ADB_REG_2_FLAG_CONTROL 11
#define ADB_REG_2_FLAG_SHIFT 10
#define ADB_REG_2_FLAG_OPTION 9
#define ADB_REG_2_FLAG_COMMAND 8
#define ADB_REG_2_FLAG_NUM_LOCK 7
#define ADB_REG_2_FLAG_SCROLL_LOCK 6
#define ADB_REG_2_FLAG_SCROLL_LOCK_LED 2
#define ADB_REG_2_FLAG_CAPS_LOCK_LED 1
#define ADB_REG_2_FLAG_NUM_LOCK_LED 0

#define ADB_REG_2_DEFAULT 0xFFFF



#endif