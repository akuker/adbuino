//---------------------------------------------------------------------------
//
//	QuokkADB ADB keyboard and mouse adapter
//
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of QuokkADB.
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
//
//  Portions of this code were originally released under a Modified BSD 
//  License. See LICENSE in the root of this repository for more info.
//
//----------------------------------------------------------------------------


#ifndef _BOARDS_QUOKKADB_H
#define _BOARDS_QUOKKADB_H

// For board detection
#define QUOKKADB_BOARD

// --- UART ---

#define PICO_DEFAULT_UART 0
#define PICO_DEFAULT_UART_TX_PIN 16


// --- FLASH ---

#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 4
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)
#endif

// Drive high to force power supply into PWM mode (lower ripple on 3V3 at light loads)
#define PICO_SMPS_MODE_PIN 23

#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 0
#endif

#endif
