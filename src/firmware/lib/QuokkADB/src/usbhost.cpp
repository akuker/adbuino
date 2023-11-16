//---------------------------------------------------------------------------
//
//	QuokkaADB ADB keyboard and mouse adapter
//
//     Copyright (C) 2019 Ha Thach (tinyusb.org)
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
//  Portions of this code were originally released under the MIT License (MIT). 
//  See LICENSE in the root of this repository for more info.
//----------------------------------------------------------------------------

#include "tusb.h"
#include <class/hid/hid_host.h>
#include "quokkadb_gpio.h"
#include "adbregisters.h"
#include "usbkbdparser.h"
#include "usb_hid_keys.h"
#include "platformkbdparser.h"
#include "platformmouseparser.h"
#include "usbmouseparser.h"
#include "adbkbdparser.h"

#define kModCmd 1
#define kModOpt 2
#define kModShift 4
#define kModControl 8
#define kModReset 16
#define kModCaps 32
#define kModDelete 64
extern uint16_t modifierkeys;
extern ADBKbdRptParser KeyboardPrs;
extern MouseRptParser MousePrs;

bool set_hid_report_ready = true;

//--------------------------------------------------------------------+
// Host HID
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
  
  (void)desc_report;
  (void)desc_len;

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  // Receive report from boot keyboard & mouse only
  // tuh_hid_report_received_cb() will be invoked when report is available
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD || itf_protocol == HID_ITF_PROTOCOL_MOUSE)
  {
    if ( !tuh_hid_receive_report(dev_addr, instance) )
    {
      // Error: cannot request report
    }
    else
    {
      if(itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) 
      {
        KeyboardPrs.AddKeyboard(dev_addr, instance);
        led_blink(2);
      } 
      else // protocol is mouse
      {
        led_blink(3);
      }
    }
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  KeyboardPrs.RemoveKeyboard(dev_addr, instance);
  led_blink(1);
}

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}

static void process_kbd_report(uint8_t dev_addr, uint8_t instance, hid_keyboard_report_t const *report)
{
  KeyboardPrs.Parse(dev_addr, instance, report);
}

static void process_mouse_report(uint8_t dev_addr, hid_mouse_report_t const * report)
{
    MousePrs.Parse(report);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch(itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      process_kbd_report(dev_addr, instance, (hid_keyboard_report_t const*) report );      
    break;

    case HID_ITF_PROTOCOL_MOUSE:
      process_mouse_report(dev_addr, (hid_mouse_report_t const*) report );
    break;

    default: break;
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    //    Cannot request report
  }
}

void tuh_hid_set_report_complete_cb(uint8_t dev_addr, uint8_t instance, uint8_t report_id, uint8_t report_type, uint16_t len)
{
  set_hid_report_ready = true;
}
