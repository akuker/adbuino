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
#pragma once
#include <stdint.h>
#include <hardware/flash.h>

#define QUOKKADB_SETTINGS_MAGIC_NUMBER 0x71DE

struct __attribute((packed)) QuokkADBSettings 
{
    uint16_t magic_number;
    uint8_t led_on: 1;
    uint8_t reserved_bits: 7;
    uint8_t reserved_bytes[253];
};

class FlashSettings
{
public:
    void init(void);
    void write_settings_page(uint8_t *buf); 
    uint8_t* read_settings_page(void);
    void save(void);
    inline QuokkADBSettings* settings() {return &_settings;}
private:
    uint32_t _capacity = 0;
    uint32_t _last_sector = 0;
    QuokkADBSettings _settings;
};

