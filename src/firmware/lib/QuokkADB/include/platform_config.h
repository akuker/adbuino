//---------------------------------------------------------------------------
//
//	QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the QuokkaADB project.
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

// Use macros for version number
#define FW_VER_NUM      "0.4.0"
#define FW_VER_SUFFIX   "beta"
#define PLATFORM_FW_VERSION FW_VER_NUM "-" FW_VER_SUFFIX 
#define PRODUCT_NAME "QuokkADB"
#define PLATFORM_FW_VER_STRING PRODUCT_NAME " firmware: " PLATFORM_FW_VERSION " " __DATE__ " " __TIME__ 

#ifndef DEFAULT_MOUSE_SENSITIVITY_DIVISOR
#define DEFAULT_MOUSE_SENSITIVITY_DIVISOR 4
#endif

#define SAVE_TO_FLASH_BLINK_COUNT 4
#define CLEAR_FLASH_BLINK_COUNT 5
