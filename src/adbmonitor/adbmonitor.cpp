#include "adb.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#define ADB_GPIO 15
#define PWR_GPIO 14

int prev_adb = -1;
int prev_pwr = -1;

extern uint8_t mousepending;
extern uint8_t kbdpending  ;
extern uint16_t mousereg0  ;
extern uint16_t kbdreg0    ;


int main()
{
    int read_val;
    stdio_init_all();

    puts("Hello, world!");

    gpio_init(ADB_GPIO);
    gpio_init(PWR_GPIO);
    gpio_set_dir(ADB_GPIO, GPIO_IN);
    gpio_set_dir(PWR_GPIO, GPIO_IN);

    while (true){
        // read_val = gpio_get(ADB_GPIO);
        // if(read_val != prev_adb){
        //     prev_adb = read_val;
        //     printf("ADB changed to %d\n", read_val);
        // }

        // read_val = gpio_get(PWR_GPIO);
        // if(read_val != prev_pwr){
        //     prev_pwr = read_val;
        //     printf("PWR changed to %d\n", read_val);
        // }
        uint8_t cmd = adb_recv_cmd(0);
        if(cmd != 0){

            // uint32_t rand_val = rand();
            // if(rand_val > 0x8000){
            //     mousereg0 = (uint16_t)(rand_val & 0xFFFF);
            //     mousepending = 1;
            //     printf("moving mouse %04X  ", mousereg0);
            // }


            printf("ADB cmd: %02X\n", cmd);
            handle_adb_cmd(cmd);
        }
    }

    sleep_ms(1);

    return 0;
}
