//---------------------------------------------------------------------------
//
//	QuokkADB ADB keyboard and mouse adapter
//
//     Copyright (C) 2023 Rabbit Hole Computing LLC
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

#include <scqueue.h>
#define BLINK_QUEUE_LENGTH 5

struct blink_event_t
{
  uint8_t times;
  uint32_t delay_ms;  
};

class BlinkLed
{
public:
    bool blink(uint8_t times, uint32_t delay_ms = 75);
    void led_on(bool force = false);
    void led_off(bool force = false);
    void poll();
protected:
    simple_circular_queue::SCQueue<blink_event_t*, BLINK_QUEUE_LENGTH> blink_queue;
    bool blinking = false;
};

extern BlinkLed blink_led;