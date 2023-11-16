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
#include "tusb.h"
#include "host/usbh.h"
#include "pico/stdio.h"
#include "platform_logmsg.h"
#include "adb.h"
#include "quokkadb_gpio.h"
#include "adbkbdparser.h"
#include "adbmouseparser.h"
#include "flashsettings.h"
#include "platform_config.h"


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
bool usb_reset = false;
bool global_debug = false;

AdbInterface adb;

ADBKbdRptParser KeyboardPrs;
ADBMouseRptParser MousePrs(KeyboardPrs);
FlashSettings setting_storage;

/*------------ Core0 setup ------------*/
void setup()
{
  set_sys_clock_khz(125000, true);
  
  led_gpio_init();
  led_blink(1);
  uart_gpio_init();
  adb_gpio_init();

  setting_storage.init();
  
  printf("%s\n", PLATFORM_FW_VER_STRING);
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
    if (MousePrs.MouseChanged())
    {
      mousereg0 = MousePrs.GetAdbRegister0();
      mousepending = 1;
    }
  }

  led_off();
  cmd = adb.ReceiveCommand(mousesrq | kbdsrq);
  if(setting_storage.settings()->led_on)
  {
    led_on();
  }  
  adb.ProcessCommand(cmd);

  if (adb_reset)
  {
    adb.Reset();
    adb_reset = false;
    usb_reset = true;
    Logmsg.println("ALL: Resetting devices");
  } 
}


/*------------ Core1 setup ------------*/

void setup1()
{
  tuh_init(0);
  led_blink(1);
}

/*------------ Core1 main loop ------------*/
void loop1()
{
      tuh_task(); // tinyusb host task

    KeyboardPrs.ChangeUSBKeyboardLEDs();
    
    if (true == usb_reset)
    {
      KeyboardPrs.Reset();
      usb_reset = false;
    }
}
