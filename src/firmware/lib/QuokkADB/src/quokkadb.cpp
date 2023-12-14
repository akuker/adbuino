//---------------------------------------------------------------------------
//
//	QuokkaADB ADB keyboard and mouse adapter
//
//     Copyright 2011 Jun WAKO <wakojun@gmail.com>
//     Copyright 2013 Shay Green <gblargg@gmail.com>
//	   Copyright (C) 2017 bbraun
//     Copyright (C) 2019 Ha Thach (tinyusb.org)
//	   Copyright (C) 2020 difegue
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of QuokkaADB.
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
//  with file. If not, see <https://www.gnu.org/licenses/>.
//
//  Portions of this code were originally released under a Modified BSD 
//  License. See LICENSE in the root of this repository for more info.
//  
//  Portions of this code were originally released under the MIT License (MIT). 
//  See LICENSE in the root of this repository for more info.
//----------------------------------------------------------------------------

#include <Arduino.h>

#include "pico/stdlib.h"
#include <pico/multicore.h>
#include "tusb.h"
#include "host/usbh.h"
#include "platform_logmsg.h"
#include "adb.h"
#include "quokkadb_gpio.h"
#include "adbkbdparser.h"
#include "adbmouseparser.h"
#include "flashsettings.h"
#include "platform_config.h"
#include "blink.h"

// Globals
extern uint8_t mousepending;
extern uint8_t kbdpending;
extern uint8_t kbdskip;
extern uint16_t kbdprev0;
extern uint16_t mousereg0;
extern uint16_t kbdreg0;
extern uint16_t kbdreg2;
extern uint8_t kbdsrq;
extern uint8_t mousesrq;
extern uint32_t kbskiptimer;
extern uint16_t modifierkeys;
extern bool adb_reset;
extern bool mouse_flush;
extern bool kbd_flush;
bool usb_reset = false;
bool usb_mouse_reset = false;
bool usb_kbd_reset = false;
bool global_debug = false;

AdbInterface adb;

ADBKbdRptParser KeyboardPrs;
ADBMouseRptParser MousePrs(KeyboardPrs);
FlashSettings setting_storage;

/*------------ Pre Core0 and Core1 setup ------------*/
void initVariant() 
{ 
  led_gpio_init();
  uart_gpio_init();
  Serial1.begin();
}

/*------------ Core0 setup ------------*/
void setup()
{
  set_sys_clock_khz(125000, true);
  blink_led.blink(1);
  adb_gpio_init();
  setting_storage.init();
  Logmsg.println(PLATFORM_FW_VER_STRING);
  srand(time_us_32());
}

/*------------ Core0 main loop ------------*/
void loop()
{
  int16_t cmd = 0;

  if (!kbdpending)
  {
    if (KeyboardPrs.PendingKeyboardEvent())
    {
      kbdreg0 = KeyboardPrs.GetAdbRegister0();
      kbdreg2 = KeyboardPrs.GetAdbRegister2();
      kbdpending = 1;

    }
  }
  
  if (!mousepending)
  {
    if (MousePrs.MouseReady())
    {
      mousepending = 1;
    }
  }

  blink_led.led_off();
  cmd = adb.ReceiveCommand(mousesrq | kbdsrq);
  if(setting_storage.settings()->led_on)
  {
    blink_led.led_on();
  }  
  adb.ProcessCommand(cmd);

  if (adb_reset)
  {
    adb.Reset();
    adb_reset = false;
    usb_reset = true;
    Logmsg.println("ALL: Resetting devices");
  } 

  if (mouse_flush)
  {
    usb_mouse_reset = true;
    mouse_flush = false;
  }

  if (kbd_flush)
  {
    usb_kbd_reset = true;
    kbd_flush = false;
  }
}


/*------------ Core1 setup ------------*/

void setup1()
{
  tuh_init(0);  
  sleep_us(500);
}

/*------------ Core1 main loop ------------*/
void loop1()
{
  tuh_task(); // tinyusb host task
  blink_led.poll();
  
  KeyboardPrs.ChangeUSBKeyboardLEDs();

  if (adb_reset)
  {
    MousePrs.Reset();
  }

  if (usb_mouse_reset)
  {
    MousePrs.Reset();
    usb_mouse_reset = false;
  }

  if (usb_kbd_reset)
  {
    KeyboardPrs.Reset();
    usb_kbd_reset = false;
  }

}
