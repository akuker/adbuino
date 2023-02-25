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

//---------------------------------------------------------------------------
//  This code emulates USB Host Shield 2.0 mouse and keyboard parsers
//  in "hidboot.h" for TinyUSB
//---------------------------------------------------------------------------
#pragma once

#include <stdint.h>
#include "tusb.h"

//----------------------------------------------------------------------------
// Keyboard handler
//----------------------------------------------------------------------------
#define UHS_HID_BOOT_KEY_ZERO           0x27
#define UHS_HID_BOOT_KEY_ENTER          0x28
#define UHS_HID_BOOT_KEY_SPACE          0x2c
#define UHS_HID_BOOT_KEY_CAPS_LOCK      0x39
#define UHS_HID_BOOT_KEY_SCROLL_LOCK    0x47
#define UHS_HID_BOOT_KEY_NUM_LOCK       0x53
#define UHS_HID_BOOT_KEY_ZERO2          0x62
#define UHS_HID_BOOT_KEY_PERIOD         0x63

#define MAX_KEYBOARDS 32

struct KBDINFO {

        struct {
                uint8_t bmLeftCtrl : 1;
                uint8_t bmLeftShift : 1;
                uint8_t bmLeftAlt : 1;
                uint8_t bmLeftGUI : 1;
                uint8_t bmRightCtrl : 1;
                uint8_t bmRightShift : 1;
                uint8_t bmRightAlt : 1;
                uint8_t bmRightGUI : 1;
        };
        uint8_t bReserved;
        uint8_t Keys[6];
};

struct KBDLEDS {
        uint8_t bmNumLock : 1;
        uint8_t bmCapsLock : 1;
        uint8_t bmScrollLock : 1;
        uint8_t bmCompose : 1;
        uint8_t bmKana : 1;
        uint8_t bmReserved : 3;
};

struct MODIFIERKEYS {
        uint8_t bmLeftCtrl : 1;
        uint8_t bmLeftShift : 1;
        uint8_t bmLeftAlt : 1;
        uint8_t bmLeftGUI : 1;
        uint8_t bmRightCtrl : 1;
        uint8_t bmRightShift : 1;
        uint8_t bmRightAlt : 1;
        uint8_t bmRightGUI : 1;
};

struct KeyboardDevices  
{
  uint8_t device_addr;
  uint8_t instance;
  bool in_use;
};

class KeyboardReportParser {
        
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

        uint8_t OemToAscii(uint8_t mod, uint8_t key);

        KeyboardDevices keyboards_list[MAX_KEYBOARDS] = {};

public:

        KeyboardReportParser() {
                kbdLockingKeys.bLeds = 0;
        };

        void Parse(uint8_t dev_addr, uint8_t instance, hid_keyboard_report_t const *report);
        bool SpecialKeyCombo(KBDINFO *cur_kbd_info);
        void SendString(const char* message);
        void AddKeyboard(uint8_t dev_addr, uint8_t instance);
        void RemoveKeyboard(uint8_t dev_addr, uint8_t instance);
        // Sets the LEDs to shared memory
        void SetUSBkeyboardLEDs(bool capslock, bool numlock, bool scrolllock);
        // Executes the LED changes from shared memory (meant to be run on the same core as tuh_task)
        void ChangeUSBKeyboardLEDs(void);

        virtual bool PendingKeyboardEvent(void);

        virtual void OnKeyDown(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) {
        };

        virtual void OnKeyUp(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) {
        };


protected:

        virtual uint8_t HandleLockingKeys(uint8_t dev_addr, uint8_t instance, uint8_t key) {
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

        virtual void OnModifierKeysChanged(uint8_t before __attribute__((unused)), uint8_t after __attribute__((unused))) {
        };



        virtual const uint8_t *getNumKeys() {
                return numKeys;
        };

        virtual const uint8_t *getSymKeysUp() {
                return symKeysUp;
        };

        virtual const uint8_t *getSymKeysLo() {
                return symKeysLo;
        };

        virtual const uint8_t *getPadKeys() {
                return padKeys;
        };
};
