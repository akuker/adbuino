/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 */

/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* The ADB code here is mostly derived from: https://github.com/tmk/tmk_keyboard
 * It was originally ADB host, and I've adapted it for device use.
 * Below is the license and credit for the original ADB host implementation.
 */
/*
Copyright 2011 Jun WAKO <wakojun@gmail.com>
Copyright 2013 Shay Green <gblargg@gmail.com>
This software is licensed with a Modified BSD License.
All of this is supposed to be Free Software, Open Source, DFSG-free,
GPL-compatible, and OK to use in both free and proprietary applications.
Additions and corrections to this file are welcome.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.
* Neither the name of the copyright holders nor the names of
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


#pragma once
#include "stdlib.h"
#include "hardware/structs/sio.h"
#include "hardware/gpio.h"

// Status LED GPIOs
#define LED_GPIO     12
#define LED_ON()    sio_hw->gpio_set = 1 << LED_GPIO
#define LED_OFF()   sio_hw->gpio_clr = 1 << LED_GPIO
#define LED_SET(x)  (x ? sio_hw->gpio_set = 1 << LED_GPIO : sio_hw->gpio_clr = 1 << LED_GPIO)

// ADB GPIOs
#define ADB_IN_GPIO   19
#define ADB_OUT_GPIO  18
#define ADB_OUT_HIGH() sio_hw->gpio_set = 1 << ADB_OUT_GPIO
#define ADB_OUT_LOW()  sio_hw->gpio_clr = 1 << ADB_OUT_GPIO


#define ADB_IN_GET() (gpio_get(ADB_IN_GPIO))


void adb_gpio_init(void);
void led_gpio_init(void);
void led_blink(uint8_t times);

inline void led_on(void) {
    LED_ON();
}

inline void led_off(void) {
    LED_OFF();
}