#pragma once

#include <stdint.h>
#include "pico/stdlib.h"


#define ADB_GPIO 15
#define PWR_GPIO 14


// The original data_lo code would just set the bit as an output
// That works for a host, since the host is doing the pullup on the ADB line,
// but for a device, it won't reliably pull the line low.  We need to actually
// set it.
#define data_lo() (gpio_put(ADB_GPIO, 1))
#define data_hi() (gpio_put(ADB_GPIO, 0))
#define data_in() (gpio_get(ADB_GPIO))
#define adb_pin_out() (gpio_set_dir(ADB_GPIO, GPIO_OUT))
#define adb_pin_in()  (gpio_set_dir(ADB_GPIO, GPIO_IN))

void handle_adb_cmd(uint8_t cmd);

static inline uint16_t wait_data_lo(uint32_t us)
{
    do {
        if ( !data_in() )
            break;
        busy_wait_us_32(1);
    }
    while ( --us );
    return us;
}
static inline uint16_t wait_data_hi(uint32_t us)
{
    do {
        if ( data_in() )
            break;
        busy_wait_us_32(1);
    }
    while ( --us );
    return us;
}

static inline void place_bit0(void)
{
    data_lo();
    busy_wait_us_32(65);
    data_hi();
    busy_wait_us_32(35);
}
static inline void place_bit1(void)
{
    data_lo();
    busy_wait_us_32(35);
    data_hi();
    busy_wait_us_32(65);
}
static inline void send_byte(uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        if (data&(0x80>>i))
            place_bit1();
        else
            place_bit0();
    }
}
static uint8_t inline adb_recv_cmd(uint8_t srq) 
{
  uint8_t bits;
  uint16_t data = 0;
  
  // find attention & start bit
  if(!wait_data_lo(5000)) return 0;
  uint16_t lowtime = wait_data_hi(1000);
  if(!lowtime || lowtime > 500) {
    return 0;
  }
  wait_data_lo(100);
  
  for(bits = 0; bits < 8; bits++) {
    uint8_t lo = wait_data_hi(130);
    if(!lo) {
      goto out;
    }
    uint8_t hi = wait_data_lo(lo);
    if(!hi) {
      goto out;
    }
    hi = lo - hi;
    lo = 130 - lo;
    
    data <<= 1;
    if(lo < hi) {
      data |= 1;
    }
  }
  
  if(srq) {
    data_lo();
    busy_wait_us_32(250);
    data_hi();
  } else {
    // Stop bit normal low time is 70uS + can have an SRQ time of 300uS
    wait_data_hi(400);
  }
  
  return data;
out:
  return 0;
}