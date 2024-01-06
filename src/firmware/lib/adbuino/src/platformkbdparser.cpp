//----------------------------------------------------------------------------
//
//  QuokkADB ADB keyboard and mouse adapter
//     Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of QuokkADB.
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
//  with this file. If not, see <https://www.gnu.org/licenses/>.
//
//  A majority of this code were originally released under a 
//  General Public License version 2 (GPL2) License.
//  See LICENSE in the root of this repository for more info.
//
//---------------------------------------------------------------------------

#include "platformkbdparser.h"
#include "usb_hid_keys.h"
#include "platform_config.h"
#include "char2usbkeycode.h"
#include "adbkbdparser.h"
#include "adbmouseparser.h"
#include "platform_logmsg.h"
#include "adb.h"

extern uint16_t modifierkeys;
extern ADBKbdRptParser KeyboardPrs;

PlatformKbdParser::PlatformKbdParser()
{
        kbdLockingKeys.bLeds = 0;
}
PlatformKbdParser::~PlatformKbdParser()
{
}

bool PlatformKbdParser::SpecialKeyCombo()
{
    // Special keycombo actions
    uint8_t special_key_count = 0;
    uint8_t special_key = 0;
    uint8_t special_keys[] = { USB_KEY_V};
    uint8_t caps_lock_down = false;
    
    for (uint8_t i = 0; i < 6; i++)
    {
            if (prevState.kbdInfo.Keys[i] == USB_KEY_CAPSLOCK) {
                    caps_lock_down = true;
            }
            for (size_t j = 0; j < sizeof(special_keys); j++)
            {
                    if (special_keys[j] == prevState.kbdInfo.Keys[i])
                    {
                            special_key_count++;
                            special_key = prevState.kbdInfo.Keys[i];
                    }
            }

    }
    
    if ( special_key_count == 1 && 
        caps_lock_down &&
        (prevState.kbdInfo.bmLeftCtrl || prevState.kbdInfo.bmRightCtrl) &&
        (prevState.kbdInfo.bmLeftShift || prevState.kbdInfo.bmRightShift)
    )
    {
        KeyboardPrs.Reset();
        switch (special_key)
        {
        case USB_KEY_V:
            SendString(PLATFORM_FW_VER_STRING);
        break;       
        }
        
        return true;
    }
    return false;
}

extern HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard;

void PlatformKbdParser::SendString(const char * message)
{
        int i = 0;
        usbkey_t key;
        // force key up on modifier keys
        TaskKeyboard(true);
        OnKeyUp(0, USB_KEY_LEFTSHIFT);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_RIGHTSHIFT);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_LEFTCTRL);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_RIGHTCTRL);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_LEFTALT);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_RIGHTALT);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_LEFTMETA);
        TaskKeyboard();
        OnKeyUp(0, USB_KEY_RIGHTMETA);
        TaskKeyboard();
        // Change Caps Lock manually as it gets ignored in the OnKeyUp function
        if (m_keyboard_events.enqueue(new KeyEvent(USB_KEY_CAPSLOCK, KeyEvent::KeyUp, 0)))
        {
            // as HandleLocking keys simply toggles the keyboard LEDs, setting it to 1
            // forces it to toggle off. 
            kbdLockingKeys.kbdLeds.bmCapsLock = 1;
            HandleLockingKeys(&HidKeyboard, USB_KEY_CAPSLOCK);
            TaskKeyboard();
        }    
        else
        {
            if (global_debug)
            {
                Logmsg.println("Warning! unable to queue CAPSLOCK key up");
            }
        }

        while(message[i] != '\0')        
        {
                key = char_to_usb_keycode(message[i++]);

                if (key.shift_down) {
                        OnKeyDown(0, USB_KEY_LEFTSHIFT);
                        TaskKeyboard();
                }

                OnKeyDown(0, key.keycode);
                TaskKeyboard();
                OnKeyUp(0, key.keycode);
                TaskKeyboard();

                if (key.shift_down) {
                        OnKeyUp(0, USB_KEY_LEFTSHIFT);
                        TaskKeyboard();
                }
        }
}
extern ADBKbdRptParser KeyboardPrs;
extern ADBMouseRptParser MousePrs;
extern uint8_t mousepending;
extern uint8_t kbdpending;
extern uint16_t mousereg0;
extern uint16_t kbdreg0;
extern uint8_t kbdsrq;
extern uint8_t mousesrq;
extern AdbInterface adb;
extern bool adb_reset;

void PlatformKbdParser::TaskKeyboard(bool first)
{       
    uint16_t cmd;
    if (first)
    {
        first = false;
        if (kbdpending == 1)
        {
            if (KeyboardPrs.PendingKeyboardEvent())
            {
                kbdreg0 = KeyboardPrs.GetAdbRegister0();
                kbdpending = 1;
            }
            cmd = adb.ReceiveCommand(kbdsrq);
            adb.ProcessCommand(cmd);
        }
    }

    do
    {    
        if (KeyboardPrs.PendingKeyboardEvent())
        {
            kbdreg0 = KeyboardPrs.GetAdbRegister0();
            kbdpending = 1;
        }
        cmd = adb.ReceiveCommand(kbdsrq);
        adb.ProcessCommand(cmd); 

    } while(kbdpending != 0);

    if (adb_reset)
    {
      adb.Reset();
      adb_reset = false;
      if (global_debug)
      {
        Logmsg.println("ALL: Resetting devices");
      }
    } 
}