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
#include <Arduino.h>
#include "platformkbdparser.h"
#include "usb_hid_keys.h"
#include "platform_config.h"
#include "char2usbkeycode.h"
#include "flashsettings.h"
#include <tusb.h>
#include "platform_logmsg.h"

#define VALUE_WITHIN(v,l,h) (((v)>=(l)) && ((v)<=(h)))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

extern uint16_t modifierkeys;
extern bool set_hid_report_ready;
extern FlashSettings setting_storage;

uint8_t inline findModifierKey(hid_keyboard_report_t const *report, const hid_keyboard_modifier_bm_t mod ) {
        return (mod & report->modifier) ? 1 : 0;
}

PlatformKbdParser::PlatformKbdParser()
{
        kbdLockingKeys.bLeds = 0;
}
PlatformKbdParser::~PlatformKbdParser()
{
}

void PlatformKbdParser::AddKeyboard(uint8_t dev_addr, uint8_t instance) {
        for(size_t i = 0; i < MAX_KEYBOARDS; i++)
        {
                if (!keyboards_list[i].in_use)
                {
                        keyboards_list[i].in_use = true;
                        keyboards_list[i].device_addr = dev_addr;
                        keyboards_list[i].instance = instance;
                        SetUSBkeyboardLEDs(kbdLockingKeys.kbdLeds.bmCapsLock, 
                                kbdLockingKeys.kbdLeds.bmNumLock,
                                kbdLockingKeys.kbdLeds.bmScrollLock
                        );
                        break;
                }
               
        }

}
void PlatformKbdParser::RemoveKeyboard(uint8_t dev_addr, uint8_t instance) {
  for(size_t i = 0; i < MAX_KEYBOARDS; i++)
  {
    if (keyboards_list[i].in_use && keyboards_list[i].device_addr == dev_addr && keyboards_list[i].instance == instance)
    {
      keyboards_list[i].in_use = false;
      break;
    }
  }
}

void PlatformKbdParser::Parse(uint8_t dev_addr, uint8_t instance, hid_keyboard_report_t const *report) {
        union {
                KBDINFO kbdInfo;
                uint8_t bInfo[sizeof (KBDINFO)];
        } current_state;
        
        KBDINFO *cur_kbd_info  = &(current_state.kbdInfo); 

        cur_kbd_info->bmLeftCtrl =   findModifierKey(report, KEYBOARD_MODIFIER_LEFTCTRL);
        cur_kbd_info->bmLeftShift =  findModifierKey(report, KEYBOARD_MODIFIER_LEFTSHIFT);
        cur_kbd_info->bmLeftAlt =    findModifierKey(report, KEYBOARD_MODIFIER_LEFTALT);
        cur_kbd_info->bmLeftGUI =    findModifierKey(report, KEYBOARD_MODIFIER_LEFTGUI);
        cur_kbd_info->bmRightCtrl =  findModifierKey(report, KEYBOARD_MODIFIER_RIGHTCTRL);
        cur_kbd_info->bmRightShift = findModifierKey(report, KEYBOARD_MODIFIER_RIGHTSHIFT);
        cur_kbd_info->bmRightAlt =   findModifierKey(report, KEYBOARD_MODIFIER_RIGHTALT);
        cur_kbd_info->bmRightGUI =   findModifierKey(report, KEYBOARD_MODIFIER_RIGHTGUI);
        memcpy(cur_kbd_info->Keys, report->keycode, 6);
        cur_kbd_info->bReserved =  report->reserved;
        

        if (PlatformKbdParser::SpecialKeyCombo(cur_kbd_info))
        {
                return;
        }

        // provide event for changed control key state
        if (prevState.bInfo[0x00] != current_state.bInfo[0x00]) {
                OnControlKeysChanged(prevState.bInfo[0x00], current_state.bInfo[0x00]);
        }

        for (uint8_t i = 2; i < 8; i++) {
                bool down = false;
                bool up = false;

                for (uint8_t j = 2; j < 8; j++) {
                        if (current_state.bInfo[i] == prevState.bInfo[j] && current_state.bInfo[i] != 1)
                                down = true;
                        if (current_state.bInfo[j] == prevState.bInfo[i] && prevState.bInfo[i] != 1)
                                up = true;
                }
                if (!down) {
                        HandleLockingKeys(dev_addr, instance, current_state.bInfo[i]);        
                        OnKeyDown(current_state.bInfo[0], current_state.bInfo[i]);
                }
                if (!up) {
                        // Ignore key up on caps lock
                        if (prevState.bInfo[i] != UHS_HID_BOOT_KEY_CAPS_LOCK) {
                                OnKeyUp(current_state.bInfo[0], prevState.bInfo[i]);
                        }
                }
        }


        // store current buttons to test against next keyboard action
        for (uint8_t i = 0; i < 8; i++)
                prevState.bInfo[i] = current_state.bInfo[i];

}
//bool tuh_hid_set_report(uint8_t dev_addr, uint8_t instance, uint8_t report_id, uint8_t report_type, void* report, uint16_t len);

void PlatformKbdParser::SetUSBkeyboardLEDs(bool capslock, bool numlock, bool scrollock){
        // Send LEDs statuses to USB keyboard
        kbdLockingKeys.kbdLeds.bmCapsLock = capslock ? 1 : 0;
        kbdLockingKeys.kbdLeds.bmNumLock = numlock ? 1 : 0;
        kbdLockingKeys.kbdLeds.bmScrollLock = scrollock ? 1 : 0;
        usb_set_leds = true;

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
                case USB_KEY_L:
                        setting_storage.settings()->led_on = ~setting_storage.settings()->led_on;
                        setting_storage.save();
                        if (setting_storage.settings()->led_on) 
                        {
                                SendString("Busy LED is on");
                        }   
                        else
                        {
                                SendString("Busy LED is off");
                        }
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
        if (m_keyboard_events.enqueue(new KeyEvent(USB_KEY_CAPSLOCK, KeyEvent::KeyUp, 0)))
        {
            // as HandleLocking keys simply toggles the keyboard LEDs, setting it to 1
            // forces it to toggle off. 
            kbdLockingKeys.kbdLeds.bmCapsLock = 0;
            SetUSBkeyboardLEDs(kbdLockingKeys.kbdLeds.bmCapsLock , kbdLockingKeys.kbdLeds.bmNumLock, kbdLockingKeys.kbdLeds.bmScrollLock);
        }    
        else
        {
            Logmsg.println("Warning! unable to queue CAPSLOCK key up");
        }

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

void PlatformKbdParser::ChangeUSBKeyboardLEDs(void)
{
        if (usb_set_leds == false) 
                return;

        static size_t i = 0;
        static uint8_t usb_kbd_leds = 0;
        // USB HID Keyboard LED bit location 0x1 - numlock, 0x2 - capslock, 0x4 - scrollock
        usb_kbd_leds = kbdLockingKeys.kbdLeds.bmNumLock ? 0x1 : 0;
        usb_kbd_leds |=  kbdLockingKeys.kbdLeds.bmCapsLock ? 0x2 : 0;
        usb_kbd_leds |= kbdLockingKeys.kbdLeds.bmScrollLock ? 0x4 : 0;

        bool try_again = true;
        if (set_hid_report_ready && keyboards_list[i].in_use) {
                set_hid_report_ready = false;
                try_again = false;
                if (!tuh_hid_set_report(
                        keyboards_list[i].device_addr, 
                        keyboards_list[i].instance,  
                        0, 
                        HID_REPORT_TYPE_OUTPUT, 
                        &(usb_kbd_leds), 
                        sizeof(usb_kbd_leds)
                )) 
                {
                        set_hid_report_ready = true;
                        printf("KBD: tuh_hid_set_report failed, dev addr: %hhx, instance: %hhx\n",
                                keyboards_list[i].device_addr, 
                                keyboards_list[i].instance);  
                        
                }        
        }

        if (!keyboards_list[i].in_use || !try_again)
        {
                i++;
        } 

        if (i >= MAX_KEYBOARDS) {
                usb_set_leds = false;
                i = 0;
        }
}

const uint8_t PlatformKbdParser::numKeys[10]  = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};
const uint8_t PlatformKbdParser::symKeysUp[12]  = {'_', '+', '{', '}', '|', '~', ':', '"', '~', '<', '>', '?'};
const uint8_t PlatformKbdParser::symKeysLo[12]  = {'-', '=', '[', ']', '\\', ' ', ';', '\'', '`', ',', '.', '/'};
const uint8_t PlatformKbdParser::padKeys[5]  = {'/', '*', '-', '+', '\r'};

uint8_t PlatformKbdParser::OemToAscii(uint8_t mod, uint8_t key) {
        uint8_t shift = (mod & 0x22);

        // [a-z]
        if (VALUE_WITHIN(key, 0x04, 0x1d)) {
                // Upper case letters
                if ((kbdLockingKeys.kbdLeds.bmCapsLock == 0 && shift) ||
                        (kbdLockingKeys.kbdLeds.bmCapsLock == 1 && shift == 0))
                        return (key - 4 + 'A');

                        // Lower case letters
                else
                        return (key - 4 + 'a');
        }// Numbers
        else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
                if (shift)
                        return ((uint8_t)pgm_read_byte(&getNumKeys()[key - 0x1e])); 
                else
                        return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
        }// Keypad Numbers
        else if(VALUE_WITHIN(key, 0x59, 0x61)) {
                if(kbdLockingKeys.kbdLeds.bmNumLock == 1)
                        return (key - 0x59 + '1');
        } else if(VALUE_WITHIN(key, 0x2d, 0x38))
                return  ((shift) ? (uint8_t)pgm_read_byte(&getSymKeysUp()[key - 0x2d]) : (uint8_t)pgm_read_byte(&getSymKeysLo()[key - 0x2d]));
        else if(VALUE_WITHIN(key, 0x54, 0x58))
                return (uint8_t)pgm_read_byte(&getPadKeys()[key - 0x54]);
        else {
                switch(key) {
                        case UHS_HID_BOOT_KEY_SPACE: return (0x20);
                        case UHS_HID_BOOT_KEY_ENTER: return ('\r'); // Carriage return (0x0D)
                        case UHS_HID_BOOT_KEY_ZERO2: return ((kbdLockingKeys.kbdLeds.bmNumLock == 1) ? '0': 0);
                        case UHS_HID_BOOT_KEY_PERIOD: return ((kbdLockingKeys.kbdLeds.bmNumLock == 1) ? '.': 0);
                }
        }
        return ( 0);
}