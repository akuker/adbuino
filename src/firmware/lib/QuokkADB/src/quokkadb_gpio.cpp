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
//  any later version.
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


#include "quokkadb_gpio.h"
#include "pico/mutex.h"
#include "hardware/gpio.h"
#include "printf/printf.h"

mutex_t led_mutex;



extern bool global_debug;
void adb_gpio_init(void) {
    gpio_init(ADB_OUT_GPIO);
    gpio_set_function(ADB_OUT_GPIO, GPIO_FUNC_SIO);
    gpio_set_dir(ADB_OUT_GPIO, GPIO_OUT);
    gpio_put(ADB_OUT_GPIO, true);

    gpio_init(ADB_IN_GPIO);
    gpio_set_dir(ADB_IN_GPIO, GPIO_IN);


}



void led_gpio_init(void) {
    gpio_init(LED_GPIO);
    gpio_set_function(LED_GPIO, GPIO_FUNC_SIO);
    gpio_set_dir(LED_GPIO, GPIO_OUT);
    LED_OFF();
    mutex_init(&led_mutex);

}

void uart_gpio_init(void) {

    uart_init(UART_PORT, UART_TX_BAUD);
    gpio_set_function(UART_TX_GPIO, GPIO_FUNC_UART);

}
    


// Blink the led n times
void led_blink(uint8_t times) {
    mutex_enter_blocking(&led_mutex);
    for (uint8_t i = 0; i < times; i++) {
        LED_ON();
        sleep_ms(75);
        LED_OFF();
        sleep_ms(75);
    }
    sleep_ms(75);    
    mutex_exit(&led_mutex);
}

