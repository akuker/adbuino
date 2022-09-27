/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 * This file is derived from adbuino:  https://github.com/akuker/adbuino
 * see main.cpp for license information
 * 
 * 26 September 2022 - Modifed by Rabbit Hole Computing LLC
 */


#include "adb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bithacks.h"
#include "usb2adb_gpio.h"
#include "tusb.h"
#include "time.h"

extern uint8_t mousepending;
extern uint8_t kbdpending;
extern uint8_t kbdskip;
extern uint16_t kbdprev0;
extern uint16_t mousereg0;
extern uint16_t kbdreg0;
extern uint8_t kbdsrq;
extern uint8_t mousesrq;
extern uint16_t modifierkeys;
extern uint32_t kbskiptimer;


inline void AdbInterface::data_lo()
{
  ADB_OUT_LOW();
}

inline void AdbInterface::data_hi()
{
  ADB_OUT_HIGH();
}
inline uint8_t AdbInterface::data_in()
{
  return ADB_IN_GET();
}
inline uint16_t AdbInterface::wait_data_lo(uint32_t us)
{
  // use 64bit time to prevent possible wrapping
  uint64_t start = time_us_64();
  uint64_t time;
  do
  {
    if (!data_in()) {
      time = time_us_64();
      break;
    }
    time = time_us_64();
  } while (us >= time - start);
  uint32_t diff = static_cast<uint32_t>(time - start);
  return us >= diff ? us - (time - start) : 0;
}
inline uint16_t AdbInterface::wait_data_hi(uint32_t us)
{
  uint64_t start = time_us_64();
  uint64_t time;
  do
  {
    if (data_in()) {
      time = time_us_64();
      break;
    }
    time = time_us_64();
  } while (us >= time - start);
  uint32_t diff = static_cast<uint32_t>(time - start);
  return us >= diff ? us - diff : 0;
}

inline void AdbInterface::place_bit0(void)
{
  data_lo();
  busy_wait_us_32(65);
  data_hi();
  busy_wait_us_32(35);
}
inline void AdbInterface::place_bit1(void)
{
  data_lo();
  busy_wait_us_32(35);
  data_hi();
  busy_wait_us_32(65);
}
inline void AdbInterface::send_byte(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
      place_bit1();
    else
      place_bit0();
  }
}


uint8_t AdbInterface::ReceiveCommand(uint8_t srq)
{
  uint8_t bits;
  uint16_t data = 0;

  // find attention & start bit
  if (!wait_data_lo(100000)) {
    // timing out looking for a signal.
    led_blink(3);
    return 0;
  }
  uint16_t lowtime = wait_data_hi(1000);
  if (!lowtime || lowtime > 500)
  {
    // malformed signal
    led_blink(4);
    sleep_ms(500);
    return 0;
  }
  wait_data_lo(100);
  for (bits = 0; bits < 8; bits++)
  {
    uint8_t lo = wait_data_hi(130);
    if (!lo)
    {
      goto out;
    }
    uint8_t hi = wait_data_lo(lo);
    if (!hi)
    {
      goto out;
    }
    hi = lo - hi;
    lo = 130 - lo;

    data <<= 1;
    if (lo < hi)
    {
      data |= 1;
    }
  }
  // Stop bit normal low time is 70uS + can have an SRQ time of 300uS
  if (srq)
  {
    data_lo();
    busy_wait_us_32(250);
    data_hi();
  }
  else
  {
 
    wait_data_hi(400);

  }
  return data;
out:
  // command interrupted
  led_blink(5);
  sleep_ms(500);
  return 0;
}

bool first_mouse = true;
bool first_keybd = true;
void AdbInterface::ProcessCommand(uint8_t cmd)
{

  uint16_t mousereg3, kbdreg3;
  


  // see if it is addressed to us
  if (((cmd >> 4) & 0x0F) == 3)
  {
    switch (cmd & 0x0F)
    {
    case 0xC: // talk register 0
      if (mousepending)
      {

        busy_wait_us_32(180); // stop to start time / interframe delay
        // adb_pin_out();
        place_bit1(); // start bit
        send_byte((mousereg0 >> 8) & 0x00FF);
        send_byte(mousereg0 & 0x00FF);
        place_bit0(); // stop bit
        // adb_pin_in();
        mousepending = 0;
        mousesrq = 0;
      }
      break;
    case 0xD: // talk register 1
      // Serial.println("MOUSE: Got request for register 1");
      break;
    case 0xE: // talk register 2
      // Serial.println("MOUSE Got request for register 2");
      break;
    case 0xF: // talk register 3
      // Serial.println("MOUSE: Got request for register 3");
      // // sets device address
      // mousereg3 = GetAdbRegister3Mouse();
      // busy_wait_us_32(180); // stop to start time / interframe delay
      // //   ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
      // adb_pin_out();
      // place_bit1(); // start bit
      // send_byte((mousereg3 >> 8) & 0x00FF);
      // send_byte(mousereg3 & 0x00FF);
      // place_bit0(); // stop bit
      // //   ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
      // adb_pin_in();
      break;
    default:
//      printf("MOUSE: Unknown cmd: %02X", cmd);
      break;
    }
  }
  else
  {
    if (mousepending) {
      mousesrq = 1;
    }
  }

  if (((cmd >> 4) & 0x0F) == 2)
  {
    volatile uint16_t modifierkeys_copy;
    uint8_t modifierkeys_msb_byte;
    uint8_t modifierkeys_lsb_byte;
    switch (cmd & 0x0F)
    {
    case 0xC: // talk register 0
      if (kbdpending)
      {
        if (kbdskip)
        {
          kbdskip = 0;
          // Skipping invalid 255 signal and sending keyup instead

          // Send a 'key released' code to avoid ADB sticking to the previous key
          kbdprev0 |= 0x80;
          kbdreg0 = (kbdprev0 << 8) | 0xFF;

          // Save timestamp
          kbskiptimer = millis();
        }
        else if (millis() - kbskiptimer < 90)
        {
          // Check timestamp and don't process the key event if it came right after a 255
          // This is meant to avoid a glitch where releasing a key sends 255->keydown instead
          // printf("Too little time since bugged keyup, skipping this keydown event");
          kbdpending = 0;
          break;
        }
        kbdsrq = 0;
        busy_wait_us_32(180); // stop to start time / interframe delay
        place_bit1(); // start bit
        send_byte((kbdreg0 >> 8) & 0x00FF);
        send_byte(kbdreg0 & 0x00FF);
        place_bit0(); // stop bit
        kbdpending = 0;
      }
      break;
    case 0xD: // talk register 1
      // Serial.println("KBD: Got request for register 1");
      break;
    case 0xE: // talk register 2
      // might be overkill as CORE0 does reading and CORE1 does writing
      modifierkeys_copy = modifierkeys;
      static_cast<uint8_t>(modifierkeys >> 8);

      modifierkeys_msb_byte = static_cast<uint8_t>(modifierkeys_copy >> 8);
      modifierkeys_lsb_byte = static_cast<uint8_t>(modifierkeys_copy);
      // if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
      // if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
      // if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;

      busy_wait_us_32(180); // stop to start time / interframe delay
      place_bit1(); // start bit
      send_byte(modifierkeys_copy);
      send_byte(modifierkeys_lsb_byte);
      place_bit0(); // stop bit
      break;
    case 0xF: // talk register 3
      // Serial.println("KBD: Got request for register 3");
      // sets device address
      kbdreg3 = GetAdbRegister3Keyboard();
      busy_wait_us_32(180); // stop to start time / interframe delay
      place_bit1(); // start bit
      send_byte((kbdreg3 >> 8) & 0x00FF);
      send_byte(kbdreg3 & 0x00FF);
      place_bit0(); // stop bit
      break;
    default:
//      printf("KBD: Unknown cmd: %02X\n", cmd);
      break;
    }
  }
  else
  {
    if (kbdpending)
      kbdsrq = 1;
  }
}

void AdbInterface::Init()
{
  // Set ADB line as input
  adb_gpio_init();
}

uint16_t AdbInterface::GetAdbRegister3Keyboard()
{
  uint16_t kbdreg3 = 0;

  // Bit 15 Reserved; must be 0
  B_UNSET(kbdreg3, 15);
  // 14      Exceptional event, device specific; always 1 if not used
  B_SET(kbdreg3, 14);
  // 13      Service Request enable; 1 = enabled
  B_UNSET(kbdreg3, 13);
  // 12      Reserved; must be 0
  B_UNSET(kbdreg3, 12);
  // 11-8      Device address
  kbdreg3 |= (0x02 << 8);
  // 7-0       Device Handler ID

  return kbdreg3;
}
uint16_t AdbInterface::GetAdbRegister3Mouse()
{
  uint16_t mousereg3 = 0;

  // Bit 15 Reserved; must be 0
  B_UNSET(mousereg3, 15);
  // 14      Exceptional event, device specific; always 1 if not used
  B_SET(mousereg3, 14);
  // 13      Service Request enable; 1 = enabled
  B_UNSET(mousereg3, 13);
  // 12      Reserved; must be 0
  B_UNSET(mousereg3, 12);
  // 11-8      Device address
  mousereg3 |= (0x03 << 8);
  // 7-0       Device Handler ID

  return mousereg3;
}