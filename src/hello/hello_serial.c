/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "adb_receive.pio.h"
// #include "adb_write.pio.h"

#define NELEMENTS(x) (sizeof(x) / sizeof(x[0]))


#define ADB_PIN 19


void push_byte(uint8_t val)
{
    // The PIO state machine will "flip" the 32 bit word
    pio_sm_put(pio0, 0, ((uint32_t)val) << 24);
}
void push_word(uint16_t val)
{
    // The PIO state machine will "flip" the 32 bit word
    pio_sm_put(pio0, 0, ((uint32_t)val) << 16);
}
 

int main()
{
    stdio_init_all();
    printf("ADB Smoke Test, using pin %d", ADB_PIN);

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    // uint offset = pio_add_program(pio, &adb_write_program);
     uint offset = pio_add_program(pio, &adb_receive_program);

    adb_receive_program_init(pio, sm, offset, ADB_PIN, 800000, 0);

    // uint16_t t = 0;
    int slow=0;
    while (1)
    {
        if(!pio_sm_is_rx_fifo_empty(pio0, 0)){
            uint32_t value = pio_sm_get(pio, 0);
            slow+=100;
            if(slow > 10){
                slow = 0;
                printf("%08X\n", value);
                fflush(stdout);
            }
        }
        else{
            slow = slow;
        }
    }
    while(1){
        // push_byte(t & 0xFF);
        push_word(0xABCD);

        // push_word(t);
        // t++;
        // printf("%d\n",t);
        sleep_ms(10);
    }
}
