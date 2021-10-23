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
#ifdef ADBUINO_DEBUG
#include "avr8-stub.h"
#include "app_api.h"
#endif
// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>
#include "usbinterface.h"
#include "usb_description.h"
#include "flexible_hid.h"
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

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

bool global_debug = true;

AdbInterface adb;
UsbInterface usb;

USB Usb;
USBHub Hub1(&Usb);
USBHub Hub2(&Usb);
USBHub Hub3(&Usb);
USBHub Hub4(&Usb);
// HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);
// HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);
// HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(&Usb);
FlexibleUsbHidDevice HidDevice(&Usb);
FlexibleUsbHidDevice HidDevice2(&Usb);
FlexibleUsbHidDevice HidDevice3(&Usb);
FlexibleUsbHidDevice HidDevice4(&Usb);
FlexibleUsbHidDevice HidDevice5(&Usb);

// HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard2(&Usb);
// HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse2(&Usb);
// HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite2(&Usb);

MouseRptParser MousePrs;
KbdRptParser KeyboardPrs;

unsigned long blink_timer = 0;
int led_state = HIGH;

void setup()
{
#ifdef ADBUINO_DEBUG
  debug_init();
#else
  Serial.begin(115200);
  while (!Serial)
    ;
#endif

  // Clear the reset signal from the USB controller
  pinMode(7, OUTPUT);
  digitalWrite(7,HIGH);

  // Setup blinking LED
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);

#ifndef ADBUINO_DEBUG
  Serial.println("Initializing ADB");
#endif
  adb.Init();
#ifndef ADBUINO_DEBUG
  Serial.println("Initializing USB");
#endif
  // usb.Init();

#ifndef ADBUINO_DEBUG
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  else 
    Serial.println("OSC started");
#else
  Usb.Init();
#endif

  HidDevice.KeyboardParser = &KeyboardPrs;
  HidDevice.MouseParser = &MousePrs;
// HidDevice.SetReportParser(0, &KeyboardPrs);
// HidDevice.SetReportParser(1, &MousePrs);
  // HidKeyboard.SetReportParser(0, &KeyboardPrs);
  // HidMouse.SetReportParser(0, &MousePrs);
  // HidComposite.SetReportParser(0, &KeyboardPrs);
  // HidComposite.SetReportParser(1, &MousePrs);
  // HidKeyboard2.SetReportParser(0, &KeyboardPrs);
  // HidMouse2.SetReportParser(0, &MousePrs);

#ifndef DRV_DEBUG_H_
  Serial.println("setup complete");
#endif
}

void update_blinker(){
  if((millis() - blink_timer) > 500){
    led_state = (led_state == HIGH) ? LOW : HIGH;
    digitalWrite(A0, led_state);
    blink_timer = millis();
  }

}

void loop()
{
  static int first_time=1;
  uint8_t cmd = 0;

  Usb.Task();

  if(( Usb.getUsbTaskState() == USB_STATE_RUNNING )){

    if (global_debug && first_time)
    {
      first_time=0;
#ifndef ADBUINO_DEBUG
      Usb.ForEachUsbDevice(&PrintAllDescriptors);
      Usb.ForEachUsbDevice(&PrintAllAddresses);
#endif
    }

    update_blinker();

    if (!mousepending)
    {
      if (MousePrs.MouseChanged())
      {
        mousereg0 = MousePrs.GetAdbRegister0();
        mousepending = 1;
      }
    }

    if (!kbdpending)
    {
      if (KeyboardPrs.PendingKeyboardEvent())
      {
        kbdreg0 = KeyboardPrs.GetAdbRegister0();
        kbdpending = 1;
      }
    }

    cmd = adb.ReceiveCommand(mousesrq | kbdsrq);
    adb.ProcessCommand(cmd);
  }
}
