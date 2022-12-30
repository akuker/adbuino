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
#include "stdlib.h"
#include "hardware/structs/sio.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"


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

#define GPIO_TEST 20

// UART out messaging
#define UART_TX_GPIO    16
#define UART_TX_BAUD    115200
#define UART_PORT       uart0

void adb_gpio_init(void);
void uart_gpio_init(void);
void led_gpio_init(void);
void led_blink(uint8_t times);


inline void led_on(void) {
    LED_ON();
}

inline void led_off(void) {
    LED_OFF();
}