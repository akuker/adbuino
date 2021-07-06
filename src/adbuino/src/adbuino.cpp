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
#include "Arduino.h"
#include <util/delay.h>
#include "adb.h"
#include "ps2kbd.h"
// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>
#include "usbinterface.h"
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <usbhub.h>
// #include "pgmstrings.h"

#define kModCmd 1
#define kModOpt 2
#define kModShift 4
#define kModControl 8
#define kModReset 16
#define kModCaps 32
#define kModDelete 64
uint8_t mousepending = 0;
uint8_t kbdpending = 0;
uint8_t kbdskip = 0;
uint16_t kbdprev0 = 0;
uint16_t mousereg0 = 0;
uint16_t kbdreg0 = 0;
uint8_t kbdsrq = 0;
uint8_t mousesrq = 0;
uint8_t modifierkeys = 0xFF;
uint32_t kbskiptimer = 0;

AdbInterface adb;
UsbInterface usb;

USB Usb;
USBHub Hub1(&Usb);
USBHub Hub2(&Usb);
USBHub Hub3(&Usb);
USBHub Hub4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

MouseRptParser MousePrs;
KbdRptParser KeyboardPrs;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Initializing ADB");
  adb.Init();
  Serial.println("Initializing USB");
  // usb.Init();

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  HidKeyboard.SetReportParser(0, &KeyboardPrs);
  HidMouse.SetReportParser(0, &MousePrs);

  // delay( 200 );

  delay(1);

  Serial.println("Initializing keyboard");
  InitPS2Keyboard();
  Serial.println("Initializing mouse");
  InitPS2Mouse();
  // Serial.println("Setting keyboard repeat");
  // SetPS2Repeat();
  // Serial.println("Setting up mouse");
  // SetupPS2Mouse();
  kPS2MouseModePort &= kPS2MouseClockPin;
  kPS2MouseOutPort |= kPS2MouseClockPin;
  kPS2MouseModePort &= kPS2MouseDataPin;
  kPS2MouseOutPort |= kPS2MouseDataPin;
  DDRB |= 1 << 5;
  PORTC &= ~(1 << 5);
  DDRC |= 1 << 5;
  PORTC &= ~(1 << 5);
  Serial.println("setup complete");
}
void loop()
{
  // uint8_t ps2key, ps2keyup, ps2extended;
  // uint8_t ps2status, ps2xmove, ps2ymove;
  uint8_t cmd = 0;
  // uint16_t us;

  // Serial.println("Run USB");
  // usb.RunTask();
  Usb.Task();

  if (!mousepending)
  {
    if (MousePrs.MouseChanged())
    {

      // Bit   Meaning
      // ----  --------------------------
      // 15    Button Status; 0=down
      // 14-8  Y move Counts (Two's compliment. Negative = up, positive = down)
      // 7     Not used. Always 1
      // 6-0   X move counts (Two's compliment. Negative = left, positive = right)
      mousereg0 = MousePrs.GetAdbRegister0();
      Serial.print("Mouse Register: ");
      Serial.println(mousereg0, HEX);
      MousePrs.ResetMouseMovement();
      mousepending = 1;
    }
  }
  // // inhibit mouse communication
  // kPS2MouseModePort |= kPS2MouseClockPin;
  // kPS2MouseOutPort &= ~kPS2MouseClockPin;

  if(!kbdpending) {
    if(KeyboardPrs.GetLastKey() != KbdRptParser::NoKey){
      kbdreg0 = KeyboardPrs.GetAdbRegister0();
      KeyboardPrs.ClearLastKey();
      kbdpending = 1;
    }
  }

  if (mousepending || kbdpending)
  {
    cmd = adb.ReceiveCommand(mousesrq | kbdsrq);
    // cmd = adb.ReceiveCommand(3);
    // if(cmd != 0)
    // {
    //   Serial.print("Received ADB command: ");
    //   Serial.println(cmd, HEX);
    // }
  }

  adb.ProcessCommand(cmd);
  // if(((cmd >> 4) & 0x0F) == 2) {
  //   uint8_t adbleds;
  //   switch(cmd & 0x0F) {
  //     case 0xC: // talk register 0
  //       if(kbdpending) {

  //         if (kbdskip) {
  //           kbdskip = 0;
  //           // Serial.println("Skipping invalid 255 signal and sending keyup instead");

  //           // Send a 'key released' code to avoid ADB sticking to the previous key
  //           kbdprev0 |= 0x80;
  //           kbdreg0 = (kbdprev0 << 8) | 0xFF;

  //           // Save timestamp
  //           kbskiptimer = millis();
  //         } else if (millis() - kbskiptimer < 90) {
  //         // Check timestamp and don't process the key event if it came right after a 255
  //         // This is meant to avoid a glitch where releasing a key sends 255->keydown instead
  //           Serial.println("Too little time since bugged keyup, skipping this keydown event");
  //           kbdpending = 0;
  //           break;
  //         }

  //         // Serial.print("Sending keycode ");
  //         // Serial.print(kbdreg0);
  //         // Serial.println(" to ADB");

  //         kbdsrq = 0;
  //         _delay_us(180); // stop to start time / interframe delay
  //         ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
  //         place_bit1(); // start bit
  //         send_byte((kbdreg0 >> 8) & 0x00FF);
  //         send_byte(kbdreg0 & 0x00FF);
  //         place_bit0(); // stop bit
  //         ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
  //         kbdpending = 0;
  //       }
  //       break;
  //     case 0xD: // talk register 1
  //       break;
  //     case 0xE: // talk register 2
  //       adbleds = 0xFF;
  //       if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
  //       if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
  //       if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;

  //       _delay_us(180); // stop to start time / interframe delay
  //       ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
  //       place_bit1(); // start bit
  //       send_byte(modifierkeys);
  //       send_byte(adbleds);
  //       place_bit0(); // stop bit
  //       ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input

  //       break;
  //     case 0xF: // talk register 3
  //       // sets device address
  //       break;
  //     default:
  //       Serial.print("Unknown cmd: ");
  //       Serial.println(cmd, HEX);
  //       break;
  // //   }
  // } else {
  //   if(kbdpending) kbdsrq = 1;
  // }
}
