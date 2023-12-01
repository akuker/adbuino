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
#include "blink.h"
#include "quokkadb_gpio.h"
#include "adb_platform.h"

BlinkLed blink_led;

bool BlinkLed::blink(uint8_t times, uint32_t delay_ms)
{
    blink_event_t* event = new blink_event_t;
    event->times = times;
    event->delay_ms = delay_ms;
    return blink_queue.enqueue(event);
}

void BlinkLed::led_on(bool force)
{
    if (force || !blinking)
    {
        LED_ON();
    }
}

void BlinkLed::led_off(bool force)
{
    if (force || !blinking)
    {
        LED_OFF();
    }
}

void BlinkLed::poll()
{
    static blink_event_t *current_event = nullptr;
    static bool on_phase = true;
    static uint8_t blinked = 0;
    static bool ending = false;
    static uint32_t start_delay;
    static uint32_t delay_period;

    if (!blinking && !blink_queue.isEmpty())
    {
        current_event = blink_queue.dequeue();
        delay_period = current_event->delay_ms;
        start_delay = millis();
        LED_ON();
        on_phase = true;
        blinking = true;
        blinked = 0;
        ending = false;
    }

    if (blinking)
    {
        if ((uint32_t)(millis() - start_delay) > delay_period)
        {
            if (ending)
            {
                // end of blink routine
                ending = false;
                delete current_event;
                blinking = false;
            }
            else if (on_phase)
            {
                on_phase = false;
                LED_OFF();
                start_delay = millis();
                if (blinked + 1 >= current_event->times)
                {
                    delay_period *= 4;
                    ending = true;
                }
            }
            else
            {
                blinked++;
                on_phase = true;
                LED_ON();
                start_delay = millis();
            }
        }
    }

    
}