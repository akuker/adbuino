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
#include <util/delay.h>
#define kModCmd      1
#define kModOpt      2
#define kModShift    4
#define kModControl  8
#define kModReset   16
#define kModCaps    32
#define kModDelete  64
uint8_t mousepending = 0;
uint8_t kbdpending   = 0;
uint8_t kbdskip      = 0;
uint16_t kbdprev0    = 0;
uint16_t mousereg0   = 0;
uint16_t kbdreg0     = 0;
uint8_t kbdsrq       = 0;
uint8_t mousesrq     = 0;
uint8_t modifierkeys = 0xFF;
uint32_t kbskiptimer = 0;
#define ADB_PORT        PORTB
#define ADB_PIN         PINB
#define ADB_DDR         DDRB
#define ADB_DATA_BIT    3
// The original data_lo code would just set the bit as an output
// That works for a host, since the host is doing the pullup on the ADB line,
// but for a device, it won't reliably pull the line low.  We need to actually
// set it.
#define data_lo() { (ADB_DDR |=  (1<<ADB_DATA_BIT)); (ADB_PORT &= ~(1<<ADB_DATA_BIT)); }
#define data_hi() (ADB_DDR &= ~(1<<ADB_DATA_BIT))
#define data_in() (ADB_PIN &   (1<<ADB_DATA_BIT))
static inline uint16_t wait_data_lo(uint16_t us)
{
    do {
        if ( !data_in() )
            break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
    return us;
}
static inline uint16_t wait_data_hi(uint16_t us)
{
    do {
        if ( data_in() )
            break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
    return us;
}
#include "ps2kbd.h"
static inline void place_bit0(void)
{
    data_lo();
    _delay_us(65);
    data_hi();
    _delay_us(35);
}
static inline void place_bit1(void)
{
    data_lo();
    _delay_us(35);
    data_hi();
    _delay_us(65);
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
    _delay_us(250);
    data_hi();
  } else {
    // Stop bit normal low time is 70uS + can have an SRQ time of 300uS
    wait_data_hi(400);
  }
  
  return data;
out:
  return 0;
}
uint8_t PS2ToADBScancode(uint8_t code, uint8_t keyup, uint8_t extended)
{
        uint8_t maccode = 0xFF;
        if(!extended) {
                switch(code) {
                case 0x1C: maccode = 0x00; break; // A
                case 0x32: maccode = 0x0B; break; // B
                case 0x21: maccode = 0x08; break; // C
                case 0x23: maccode = 0x02; break; // D
                case 0x24: maccode = 0x0E; break; // E
                case 0x2B: maccode = 0x03; break; // F
                case 0x34: maccode = 0x05; break; // G
                case 0x33: maccode = 0x04; break; // H
                case 0x43: maccode = 0x22; break; // I
                case 0x3B: maccode = 0x26; break; // J
                case 0x42: maccode = 0x28; break; // K
                case 0x4B: maccode = 0x25; break; // L
                case 0x3A: maccode = 0x2E; break; // M
                case 0x31: maccode = 0x2D; break; // N
                case 0x44: maccode = 0x1F; break; // O
                case 0x4D: maccode = 0x23; break; // P
                case 0x15: maccode = 0x0C; break; // Q
                case 0x2D: maccode = 0x0F; break; // R
                case 0x1B: maccode = 0x01; break; // S
                case 0x2C: maccode = 0x11; break; // T
                case 0x3C: maccode = 0x20; break; // U
                case 0x2A: maccode = 0x09; break; // V
                case 0x1D: maccode = 0x0D; break; // W
                case 0x22: maccode = 0x07; break; // X
                case 0x35: maccode = 0x10; break; // Y
                case 0x1A: maccode = 0x06; break; // Z
                case 0x45: maccode = 0x1D ;break; // 0
                case 0x16: maccode = 0x12; break; // 1
                case 0x1E: maccode = 0x13; break; // 2
                case 0x26: maccode = 0x14; break; // 3
                case 0x25: maccode = 0x15; break; // 4
                case 0x2E: maccode = 0x17; break; // 5
                case 0x36: maccode = 0x16; break; // 6
                case 0x3D: maccode = 0x1A; break; // 7
                case 0x3E: maccode = 0x1C; break; // 8
                case 0x46: maccode = 0x19; break; // 9
                case 0x0E: maccode = 0x32; break; // `
                case 0x4E: maccode = 0x1B; break; // -
                case 0x55: maccode = 0x18; break; // =
                case 0x5D: maccode = 0x2A; break; // backslash
                case 0x66: maccode = 0x33; break; // bkspc
                case 0x29: maccode = 0x31; break; // space
                case 0x0D: maccode = 0x30; break; // tab
                case 0x58: maccode = 0x39; break; // caps
                case 0x12: maccode = 0x38; break; // left shift
                case 0x59: maccode = 0x7B; break; // right shift
                case 0x11: maccode = 0x3A; break; // left alt = opt
                case 0x5A: maccode = 0x24; break; // enter
                case 0x41: maccode = 0x2B; break; // ,
                case 0x49: maccode = 0x2F; break; // .
                case 0x4A: maccode = 0x2C; break; // /
                case 0x14: maccode = 0x36; break; // L control
                case 0x76: maccode = 0x35; break; // ESC
                case 0x52: maccode = 0x27; break; // '
                case 0x4C: maccode = 0x29; break; // ;
                case 0x7C: maccode = 0x43; break; // keypad *
                case 0x7B: maccode = 0x4E; break; // keypad -
                case 0x79: maccode = 0x45; break; // keypad +
                case 0x71: maccode = 0x41; break; // keypad .
                case 0x70: maccode = 0x52; break; // keypad 0
                case 0x69: maccode = 0x53; break; // keypad 1
                case 0x72: maccode = 0x54; break; // keypad 2
                case 0x7A: maccode = 0x55; break; // keypad 3
                case 0x6B: maccode = 0x56; break; // keypad 4
                case 0x73: maccode = 0x57; break; // keypad 5
                case 0x74: maccode = 0x58; break; // keypad 6
                case 0x6C: maccode = 0x59; break; // keypad 7
                case 0x75: maccode = 0x5B; break; // keypad 8
                case 0x7D: maccode = 0x5C; break; // keypad 9
                case 0x05: maccode = 0x7A; break; // F1
                case 0x06: maccode = 0x78; break; // F2
                case 0x04: maccode = 0x63; break; // F3
                case 0x0C: maccode = 0x76; break; // F4
                case 0x03: maccode = 0x60; break; // F5
                case 0x0B: maccode = 0x61; break; // F6
                case 0x83: maccode = 0x62; break; // F7
                case 0x0A: maccode = 0x64; break; // F8
                case 0x01: maccode = 0x65; break; // F9
                case 0x09: maccode = 0x6D; break; // F10
                case 0x78: maccode = 0x67; break; // F11
                case 0x07: maccode = 0x6F; break; // F12
                case 0x54: maccode = 0x21; break; // [
                case 0x5B: maccode = 0x1E; break; // ]
                default: break;
                }
        } else {
                switch(code) {
                case 0x1F: maccode = 0x37; break; // left windows key = cmd
                case 0x27: maccode = 0x37; break; // right windows key = cmd
                case 0x11: maccode = 0x7C; break; // right alt = cmd
                case 0x5A: maccode = 0x4C; break; // keypad enter
                case 0x4A: maccode = 0x4B; break;  // keypad /
                case 0x75: maccode = 0x3E; break;  // up -> keypad up
                case 0x6B: maccode = 0x3B; break;  // left arrow
                case 0x72: maccode = 0x3D; break;  // down arrow
                case 0x74: maccode = 0x3C; break;  // right arrow
                case 0x14: maccode = 0x7D; break; // R control
                case 0x7A: maccode = 0x79; break; // PG DN
                case 0x7D: maccode = 0x74; break; // PG UP
                case 0x6C: maccode = 0x73; break; // Home
                case 0x69: maccode = 0x77; break; // End
                case 0x71: maccode = 0x75; break; // Del
                case 0x70: maccode = 0x72; break; // Insert/Help
                }
        }
#define MODKEYSET(y) keyup ? (modifierkeys |= (y)) : (modifierkeys &= ~(y))
        switch(maccode) {
        case 0x37: MODKEYSET(kModCmd); break; 
        case 0x7C:
        case 0x3A: MODKEYSET(kModOpt); break; 
        case 0x7B:
        case 0x38: MODKEYSET(kModShift); break; 
        case 0x7D:
        case 0x36: MODKEYSET(kModControl); break;
        case 0x39: MODKEYSET(kModCaps); break;
        case 0x33: MODKEYSET(kModDelete); break;
        }
#if PWRBUTTON
        if((!(modifierkeys & kModShift)) && (maccode == 0x35)) {
                MODKEYSET(kModReset);
                if(keyup) {
                        GPIO_SetBits(GPIOB, kADBPowerPin);
                        USART2_SendString("Power key up\n\r");
                        __disable_irq();
                        adbkey[adbend] = 0xFF;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0xFF;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0xFF;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0x38;
                        adbend = (adbend+1) % kADBBuffSize;
                        adb.senddata |= (1<<2);
                        __enable_irq();
                } else {
                        GPIO_ResetBits(GPIOB, kADBPowerPin);
                        USART2_SendString("Power key down\n\r");
                        __disable_irq();
                        adbkey[adbend] = 0x38 | 0x80;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0x7F;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0x7F;
                        adbend = (adbend+1) % kADBBuffSize;
                        adbkey[adbend] = 0x7F;
                        adbend = (adbend+1) % kADBBuffSize;
                        adb.senddata |= (1<<2);
                        __enable_irq();
                }
                return 0x7F;
        }
#endif
        if(keyup) {
                maccode |= 0x80;
        }

        if (maccode == 0xFF) {
          // 255 codes are invalid(bad keyboard?), set the skip flag
          kbdskip = 1;
        } else {
          // Save valid codes so we can fallback to them in case of 255
          kbdprev0 = maccode;
        }

        kbdreg0 = (maccode << 8) | 0xFF;
        kbdpending = 1;
        return maccode;
}
void setup() {
  Serial.begin(19200);
  
  // Set ADB line as input
  ADB_DDR &= ~(1<<ADB_DATA_BIT);
  
  Serial.println("Initializing keyboard");
  InitPS2Keyboard();
  Serial.println("Initializing mouse");
  InitPS2Mouse();
  Serial.println("Setting keyboard repeat");
  SetPS2Repeat();
  Serial.println("Setting up mouse");
  SetupPS2Mouse();
  kPS2MouseModePort &= kPS2MouseClockPin;
  kPS2MouseOutPort |= kPS2MouseClockPin;
  kPS2MouseModePort &= kPS2MouseDataPin;
  kPS2MouseOutPort |= kPS2MouseDataPin;
  DDRB |= 1<<5;
  PORTC &= ~(1<<5);
  DDRC |= 1<<5;
  PORTC &= ~(1<<5);
  Serial.println("adbpoll setup");  
}
void loop() {
  uint8_t ps2key, ps2keyup, ps2extended;
  uint8_t ps2status, ps2xmove, ps2ymove;
  uint8_t cmd = 0;
  uint16_t us;
  
  if(!mousepending) {
    // uninhibit mouse communication
    kPS2MouseModePort &= ~kPS2MouseClockPin;
    kPS2MouseOutPort |= kPS2MouseClockPin;
    delayMicroseconds(5);  // Wait for the line to actually float hi before testing
    // line must be high for at least 50uS before device can send
    us = 100;
    do {
      if ( !(kPS2MouseInPort & kPS2MouseClockPin) )
        break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
  
    if(us) {
      ps2status = ps2xmove = ps2ymove = 0;
      if(HandlePS2Mouse(&ps2status, &ps2xmove, &ps2ymove) == 0) {
        mousereg0 = 0x80;
        if(!(ps2status & 0x01)) mousereg0 |= 0x8000;
        if(ps2ymove) {
          mousereg0 |= (~(ps2status) & 0x20) << 9;
          mousereg0 |= (((~(ps2ymove))+1) & 0x3F) << 8;
        }
        if(ps2xmove) {
          mousereg0 |= (ps2status & 0x10) << 2;
          mousereg0 |= (ps2xmove & 0x3F);
        }
        mousepending = 1;
      }
    }
  }
  // inhibit mouse communication
  kPS2MouseModePort |= kPS2MouseClockPin;
  kPS2MouseOutPort &= ~kPS2MouseClockPin;
  
  if(!kbdpending) {
    kPS2KbdModePort &= ~kPS2KbdClockPin;
    kPS2KbdOutPort |= kPS2KbdClockPin;
    delayMicroseconds(5);
    us = 100;
    do {
      if ( !(kPS2KbdInPort & kPS2KbdClockPin) )
        break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
    if(us) {
      if(HandlePS2Keyboard(&ps2key, &ps2keyup, &ps2extended)) {
        PS2ToADBScancode(ps2key, ps2keyup, ps2extended);
        Serial.print("Transmitting adb maccode ");
        Serial.println(kbdreg0);
       }
    }
  }
  kPS2KbdModePort |= kPS2KbdClockPin;
  kPS2KbdOutPort &= ~kPS2KbdClockPin;
  if(mousepending || kbdpending) {
    cmd = adb_recv_cmd(mousesrq|kbdsrq);
  }
  // see if it is addressed to us
  if(((cmd >> 4) & 0x0F) == 3) {
    switch(cmd & 0x0F) {
      case 0xC: // talk register 0
        if(mousepending) {
          _delay_us(180); // stop to start time / interframe delay
          ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
          place_bit1(); // start bit
          send_byte((mousereg0 >> 8) & 0x00FF);
          send_byte(mousereg0 & 0x00FF);
          place_bit0(); // stop bit
          ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
          mousepending = 0;
          mousesrq = 0;
        }
        break;
      default:
        Serial.print("Unknown cmd: ");
        Serial.println(cmd, HEX);
        break;
    }
  } else {
    if(mousepending) mousesrq = 1;
  }
  
  if(((cmd >> 4) & 0x0F) == 2) {
    uint8_t adbleds;
    switch(cmd & 0x0F) {
      case 0xC: // talk register 0
        if(kbdpending) {
          
          if (kbdskip) {
            kbdskip = 0;
            // Serial.println("Skipping invalid 255 signal and sending keyup instead");

            // Send a 'key released' code to avoid ADB sticking to the previous key
            kbdprev0 |= 0x80;
            kbdreg0 = (kbdprev0 << 8) | 0xFF;

            // Save timestamp 
            kbskiptimer = millis();
          } else if (millis() - kbskiptimer < 90) {
          // Check timestamp and don't process the key event if it came right after a 255
          // This is meant to avoid a glitch where releasing a key sends 255->keydown instead
            Serial.println("Too little time since bugged keyup, skipping this keydown event");
            kbdpending = 0;
            break;
          } 
          
          // Serial.print("Sending keycode ");
          // Serial.print(kbdreg0);
          // Serial.println(" to ADB");
          
          kbdsrq = 0;
          _delay_us(180); // stop to start time / interframe delay
          ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
          place_bit1(); // start bit
          send_byte((kbdreg0 >> 8) & 0x00FF);
          send_byte(kbdreg0 & 0x00FF);
          place_bit0(); // stop bit
          ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
          kbdpending = 0;
        }
        break;
      case 0xD: // talk register 1
        break;
      case 0xE: // talk register 2
        adbleds = 0xFF;
        if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
        if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
        if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;
        
        _delay_us(180); // stop to start time / interframe delay
        ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
        place_bit1(); // start bit
        send_byte(modifierkeys);
        send_byte(adbleds);
        place_bit0(); // stop bit
        ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
        
        break;
      case 0xF: // talk register 3
        // sets device address
        break;
      default:
        Serial.print("Unknown cmd: ");
        Serial.println(cmd, HEX);
        break;
    }
  } else {
    if(kbdpending) kbdsrq = 1;
  }
}
