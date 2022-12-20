//---------------------------------------------------------------------------
//
//	ADBuino ADB keyboard and mouse adapter
//
//     Copyright 2011 Jun WAKO <wakojun@gmail.com>
//     Copyright 2013 Shay Green <gblargg@gmail.com>
//	   Copyright (C) 2017 bbraun
//	   Copyright (C) 2020 difegue
//	   Copyright (C) 2021-2022 akuker
//
//  This file is part of ADBuino.
//
//  ADBuino is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
// any later version.
//
//  ADBuino is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with ADBuino. If not, see <https://www.gnu.org/licenses/>.
//
//  Portions of this code were originally released under a Modified BSD 
//  License. See LICENSE in the root of this repository for more info.
//
//----------------------------------------------------------------------------
#include "Arduino.h"
#include <util/delay.h>
#include "adb.h"

// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>
#include "usbinterface.h"
#include "usb_description.h"
// Satisfy the IDE, which needs to see the include statement in the ino too.
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
extern bool adbcollision;

bool global_debug = false;

AdbInterface adb;
UsbInterface usb;

USB Usb;
USBHub Hub1(&Usb);
USBHub Hub2(&Usb);
USBHub Hub3(&Usb);
USBHub Hub4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

ADBMouseRptParser MousePrs;
ADBKbdRptParser KeyboardPrs;

unsigned long blink_timer = 0;
int led_state = HIGH;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // Clear the reset signal from the USB controller
  pinMode(7, OUTPUT);
  digitalWrite(7,HIGH);

  // Setup blinking LED
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);

  Serial.println("Initializing ADB");
  adb.Init();
  Serial.println("Initializing USB");
  // usb.Init();

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  else 
    Serial.println("OSC started");

  HidKeyboard.SetReportParser(0, &KeyboardPrs);
  HidMouse.SetReportParser(0, &MousePrs);

  Serial.println("setup complete");
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
      Usb.ForEachUsbDevice(&PrintAllDescriptors);
      Usb.ForEachUsbDevice(&PrintAllAddresses);
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
