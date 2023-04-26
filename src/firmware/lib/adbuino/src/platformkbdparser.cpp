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

extern uint16_t modifierkeys;
extern bool set_hid_report_ready;



PlatformKbdParser::PlatformKbdParser()
{
        kbdLockingKeys.bLeds = 0;
}
PlatformKbdParser::~PlatformKbdParser()
{
}

bool PlatformKbdParser::SpecialKeyCombo(KBDINFO *cur_kbd_info)
{
        // Special keycombo actions
        uint8_t special_key_count = 0;
        uint8_t special_key = 0;
        uint8_t special_keys[] = { USB_KEY_V, USB_KEY_L};
        uint8_t caps_lock_down = false;
        
        for (uint8_t i = 0; i < 6; i++)
        {
                if (cur_kbd_info->Keys[i] == USB_KEY_CAPSLOCK) {
                        caps_lock_down = true;
                }
                for (size_t j = 0; j < sizeof(special_keys); j++)
                {
                        if (special_keys[j] == cur_kbd_info->Keys[i])
                        {
                                special_key_count++;
                                special_key = cur_kbd_info->Keys[i];
                        }
                }

        }
        
        
        if (    special_key_count == 1 && 
                caps_lock_down &&
                (cur_kbd_info->bmLeftCtrl || cur_kbd_info->bmRightCtrl) &&
                (cur_kbd_info->bmLeftShift || cur_kbd_info->bmRightShift)
        )
        {
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

void PlatformKbdParser::SendString(const char * message)
{
        int i = 0;
        usbkey_t key;

        // force key up on modifier keys
        while(PendingKeyboardEvent());
        OnKeyUp(0, USB_KEY_LEFTSHIFT);
        OnKeyUp(0, USB_KEY_RIGHTSHIFT);
        OnKeyUp(0, USB_KEY_LEFTCTRL);
        OnKeyUp(0, USB_KEY_RIGHTCTRL);
        OnKeyUp(0, USB_KEY_LEFTALT);
        OnKeyUp(0, USB_KEY_RIGHTALT);
        OnKeyUp(0, USB_KEY_CAPSLOCK);
        OnKeyUp(0, USB_KEY_LEFTMETA);
        OnKeyUp(0, USB_KEY_RIGHTMETA);

        while(message[i] != '\0')        
        {
                while(PendingKeyboardEvent());

                key = char_to_usb_keycode(message[i++]);

                if (key.shift_down) {
                        OnKeyDown(0, USB_KEY_LEFTSHIFT);
                }

                OnKeyDown(0, key.keycode);

                OnKeyUp(0, key.keycode);
                
                if (key.shift_down) {
                        OnKeyUp(0, USB_KEY_LEFTSHIFT);
                }
        }
}
