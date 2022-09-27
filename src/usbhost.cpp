/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 * This file is derived from Ha Thach's (tinyusb.org) TinyUSB
 * 
 * 26 September 2022 - Modifed by Rabbit Hole Computing LLC
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "tusb.h"
#include "usb2adb_gpio.h"
#include "adbregisters.h"
#include "usbkbdparser.h"
#include "usb_hid_keys.h"
#include "usbmouseparser.h"

#define kModCmd 1
#define kModOpt 2
#define kModShift 4
#define kModControl 8
#define kModReset 16
#define kModCaps 32
#define kModDelete 64

extern uint16_t modifierkeys;
extern KbdRptParser KeyboardPrs;
extern MouseRptParser MousePrs;

static bool capslock_status   = false;
static bool numlock_status    = false;
static bool scrolllock_status = false;
static bool delete_status     = false;

static bool ctrl_left_keyup   = true;
static bool ctrl_right_keyup  = true;
static bool alt_left_keyup    = true;
static bool alt_right_keyup   = true;
static bool os_left_keyup     = true;
static bool os_right_keyup    = true;
static bool shift_left_keyup  = true;
static bool shift_right_keyup = true;

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

  const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
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
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
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

static void process_modifier_key(hid_keyboard_modifier_bm_t mod_key, hid_keyboard_report_t const *report, 
                                  const uint8_t hid_key, bool* key_up, const uint8_t adb_flag, uint16_t* modifiers ) {
  if (mod_key & report->modifier) {
    *modifiers |= 1 << ADB_REG_2_FLAG_CONTROL;
    if (*key_up) {
      *key_up = false;
      KeyboardPrs.OnKeyDown(0,  hid_key);
    }
  }
  else {
    if (!*key_up) {
      *key_up = true;
      KeyboardPrs.OnKeyUp(0, hid_key);
    }
  }
}

static void process_kbd_report(uint8_t dev_addr, hid_keyboard_report_t const *report)
{
  (void) dev_addr;
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released
  bool flush = false;

  // process modifier keys from USB HID for ABD
  uint16_t adb_modifiers = 0;

  process_modifier_key(KEYBOARD_MODIFIER_LEFTCTRL,   report, USB_KEY_LEFTCTRL,   &ctrl_left_keyup,   ADB_REG_2_FLAG_CONTROL, &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_RIGHTCTRL,  report, USB_KEY_RIGHTCTRL,  &ctrl_right_keyup,  ADB_REG_2_FLAG_CONTROL, &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_LEFTGUI,    report, USB_KEY_LEFTMETA,   &os_left_keyup,     ADB_REG_2_FLAG_COMMAND, &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_RIGHTGUI,   report, USB_KEY_RIGHTMETA,  &os_right_keyup,    ADB_REG_2_FLAG_COMMAND, &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_LEFTALT,    report, USB_KEY_LEFTALT,    &alt_left_keyup,    ADB_REG_2_FLAG_OPTION,  &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_RIGHTALT,   report, USB_KEY_RIGHTALT,   &alt_right_keyup,   ADB_REG_2_FLAG_OPTION,  &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_LEFTSHIFT,  report, USB_KEY_LEFTSHIFT,  &shift_left_keyup,  ADB_REG_2_FLAG_SHIFT,   &adb_modifiers);
  process_modifier_key(KEYBOARD_MODIFIER_RIGHTSHIFT, report, USB_KEY_RIGHTSHIFT, &shift_right_keyup, ADB_REG_2_FLAG_SHIFT,   &adb_modifiers);

  // find released keys
  uint8_t released_keys[] = {0, 0, 0 ,0 ,0 ,0};
  size_t released_key_index = 0;
  for (uint8_t prev_key_index = 0; prev_key_index < 6; prev_key_index++) {
    uint8_t prev_key = prev_report.keycode[prev_key_index];
    if (prev_key && !find_key_in_report(report, prev_key)) {
      // The Apple delete key is a modifier key for ABD, but a normal key for USB
      // We need to keep track of when it first pressed and released
      if (prev_key == HID_KEY_BACKSPACE) {
        delete_status = false;
      }
      // skip releasing locking keys
      else if(prev_key == HID_KEY_CAPS_LOCK) {
        continue;
      }
      // Add released keys to list
      released_keys[released_key_index++] = prev_key;
    }
  }

  // Queue released keys
  for (uint8_t key_index = 0; key_index < 6; key_index++) {
    if (released_keys[key_index]) {
      KeyboardPrs.OnKeyUp(0, released_keys[key_index]);
    }
    else {
      break;
    }
  }

  for(uint8_t i=0; i<6; i++) {
    uint8_t keycode = report->keycode[i];
    if ( keycode )
    {
      
      if ( find_key_in_report(&prev_report, keycode) )
      {
        if (HID_KEY_BACKSPACE == keycode) {
          delete_status = true;
        }
        // exist in previous report means the current key is holding
      } else
      {
        // not existed in previous report means the current key is pressed   
        
        // Process lockable keys on first press
        switch (keycode) {
        case HID_KEY_CAPS_LOCK :
          capslock_status = !capslock_status;
          if (capslock_status) KeyboardPrs.OnKeyDown(0, keycode);
          else  KeyboardPrs.OnKeyUp(0, keycode);
          break;
         // scroll lock is weird, not on the keyboard but does exist in the modifier register
        // case HID_KEY_SCROLL_LOCK :
        //   scrolllock_status = !scrolllock_status;
        //   break;

        // case fall through to add num lock and backspace (Mac delete) to OnKeyDown
        case HID_KEY_NUM_LOCK :
          numlock_status = !numlock_status;
        case HID_KEY_BACKSPACE :
          delete_status = true;
        default :
          KeyboardPrs.OnKeyDown(0, keycode);
          break; 
        }

      }
    }
  }

  if (capslock_status)   adb_modifiers |= 1 << ADB_REG_2_FLAG_CAPS_LOCK   | 1 << ADB_REG_2_FLAG_CAPS_LOCK_LED;
  if (numlock_status)    adb_modifiers |= 1 <<  ADB_REG_2_FLAG_NUM_LOCK   | 1 <<  ADB_REG_2_FLAG_NUM_LOCK_LED;
  if (scrolllock_status) adb_modifiers |= 1 << ADB_REG_2_FLAG_SCROLL_LOCK | 1 <<  ADB_REG_2_FLAG_SCROLL_LOCK_LED;
  if (delete_status)     adb_modifiers |= 1 << ADB_REG_2_FLAG_DELETE;


  modifierkeys = ~adb_modifiers;
  prev_report = *report;
}

static void process_mouse_report(uint8_t dev_addr, hid_mouse_report_t const * report)
{
  static bool last_left_button_down_status = false;
  static bool last_right_button_down_status = false;
  MOUSEINFO mouse_info;

  mouse_info.bmLeftButton = !!(report->buttons & MOUSE_BUTTON_LEFT);
  mouse_info.bmRightButton = !!(report->buttons & MOUSE_BUTTON_RIGHT);
  mouse_info.dX = report->x;
  mouse_info.dY = report->y;

  MousePrs.OnMouseMove(&mouse_info);

  // change to mouse left button down
  if (!last_left_button_down_status && mouse_info.bmLeftButton) {
    last_left_button_down_status = true;
    MousePrs.OnLeftButtonDown(&mouse_info);
  }

  // change to mouse left button up
  if (last_left_button_down_status && !mouse_info.bmLeftButton) {
    last_left_button_down_status = false;
    MousePrs.OnLeftButtonUp(&mouse_info);
  }

  if (!last_right_button_down_status && mouse_info.bmRightButton) {
    last_right_button_down_status = true;
    MousePrs.OnRightButtonDown(&mouse_info);
  }

  if (last_right_button_down_status && !mouse_info.bmRightButton) {
    last_right_button_down_status = false;
    MousePrs.OnRightButtonUp(&mouse_info);
  }
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  static uint8_t tu_leds = 0;

  switch(itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      process_kbd_report(dev_addr, (hid_keyboard_report_t const*) report );  
      // set LEDs on the keyboard
      tu_leds = 0;
      if (capslock_status)   tu_leds |= KEYBOARD_LED_CAPSLOCK;
      if (numlock_status)    tu_leds |= KEYBOARD_LED_NUMLOCK;
      if (scrolllock_status) tu_leds |= KEYBOARD_LED_SCROLLLOCK;
      tuh_hid_set_report(dev_addr, instance,  0 , HID_REPORT_TYPE_OUTPUT, &tu_leds, sizeof(tu_leds));
    
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

bool tuh_hid_set_report(uint8_t dev_addr, uint8_t instance, uint8_t report_id, uint8_t report_type, void* report, uint16_t len);

