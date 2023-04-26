//----------------------------------------------------------------------------
//
//  ADBuino ADB keyboard and mouse adapter
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

//---------------------------------------------------------------------------
//  This code adds some features to hidboot that exist in QuokkADB
//---------------------------------------------------------------------------
#pragma once

#include <hidboot.h>

#define MAX_KEYBOARDS 32

struct KeyboardDevices  
{
  uint8_t device_addr;
  uint8_t instance;
  bool in_use;
};


class PlatformKbdParser : public KeyboardReportParser {
        
        static const uint8_t numKeys[10];
        static const uint8_t symKeysUp[12];
        static const uint8_t symKeysLo[12];
        static const uint8_t padKeys[5];

protected:

        union {
                KBDINFO kbdInfo;
                uint8_t bInfo[sizeof (KBDINFO)];
        } prevState;

        union {
                KBDLEDS kbdLeds;
                uint8_t bLeds;
        } kbdLockingKeys;

        bool usb_set_leds = false;


        KeyboardDevices keyboards_list[MAX_KEYBOARDS] = {};

public:

        PlatformKbdParser();
        virtual ~PlatformKbdParser();
        bool SpecialKeyCombo(KBDINFO *cur_kbd_info);
        void SendString(const char* message);
        void AddKeyboard(uint8_t dev_addr, uint8_t instance);
        void RemoveKeyboard(uint8_t dev_addr, uint8_t instance);
        // Sets the LEDs to shared memory
        void SetUSBkeyboardLEDs(bool capslock, bool numlock, bool scrolllock);
        // Executes the LED changes from shared memory (meant to be run on the same core as tuh_task)
        void ChangeUSBKeyboardLEDs(void);

        virtual bool PendingKeyboardEvent() = 0;

        virtual void OnKeyDown(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) = 0;

        virtual void OnKeyUp(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) = 0;


protected:

        uint8_t HandleLockingKeys(uint8_t dev_addr, uint8_t instance, uint8_t key) {
                uint8_t old_keys = kbdLockingKeys.bLeds;

                switch(key) {
                        case UHS_HID_BOOT_KEY_NUM_LOCK:
                                kbdLockingKeys.kbdLeds.bmNumLock = ~kbdLockingKeys.kbdLeds.bmNumLock;
                                break;
                        case UHS_HID_BOOT_KEY_CAPS_LOCK:
                                kbdLockingKeys.kbdLeds.bmCapsLock = ~kbdLockingKeys.kbdLeds.bmCapsLock;
                                break;
                        case UHS_HID_BOOT_KEY_SCROLL_LOCK:
                                kbdLockingKeys.kbdLeds.bmScrollLock = ~kbdLockingKeys.kbdLeds.bmScrollLock;
                                break;
                }

                if(old_keys != kbdLockingKeys.bLeds ) {
                        bool numlock = !!(kbdLockingKeys.kbdLeds.bmNumLock);
                        bool capslock = !!(kbdLockingKeys.kbdLeds.bmCapsLock);
                        bool scrolllock = !!(kbdLockingKeys.kbdLeds.bmScrollLock);
                        SetUSBkeyboardLEDs(capslock, numlock, scrolllock);                        
                }

                return 0;
        };

        virtual void OnModifierKeysChanged(uint8_t before __attribute__((unused)), uint8_t after __attribute__((unused))) = 0;

};
