//----------------------------------------------------------------------------
//
//  QuokkADB ADB keyboard and mouse adapter
//	   Copyright (C) 2021-2022 bbraun
//	   Copyright (C) 2021-2022 akuker
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
//----------------------------------------------------------------------------


#pragma once

#include <stdint.h>

class AdbInterface {
  public:
    void Init();
    uint8_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(uint8_t cmd);

    protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();

  private:
    void data_lo();
    void data_hi();
    uint8_t data_in();


    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    void place_bit0(void);
    void place_bit1(void);
    void send_byte(uint8_t data);
};
