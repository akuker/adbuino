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
#include "scqueue.h"

class KeyEvent
{
public:
    static const uint8_t NoKey = 0xFF;
    static const uint8_t KeyDown = 0x01;
    static const uint8_t KeyUp = 0x02;
    inline uint8_t GetKeycode() { return m_keycode; }
    inline bool IsKeyUp() { return m_key_updown == KeyUp; }
    inline bool IsKeyDown() { return m_key_updown == KeyDown; }
    KeyEvent(uint8_t KeyCode, uint8_t KeyUpDown, uint8_t mod)
    {
        m_key_updown = KeyUpDown;
        m_keycode = KeyCode;
        m_mod = mod;
    }

protected:
    uint8_t m_keycode;
    uint8_t m_key_updown;
    uint8_t m_mod;
};

using simple_circular_queue::SCQueue;

#define KEYBOARD_QUEUE_CAPACITY (20)

class PlatformKbdParser : public KeyboardReportParser {
        
        static const uint8_t numKeys[10];
        static const uint8_t symKeysUp[12];
        static const uint8_t symKeysLo[12];
        static const uint8_t padKeys[5];

public:

        PlatformKbdParser();
        virtual ~PlatformKbdParser();
        void TaskKeyboard(bool first = false);
        bool SpecialKeyCombo();
        void SendString(const char* message);
        virtual bool PendingKeyboardEvent() = 0;
        virtual void OnKeyDown(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) = 0;

        virtual void OnKeyUp(uint8_t mod __attribute__((unused)), uint8_t key __attribute__((unused))) = 0;
        
        virtual void OnControlKeysChanged(uint8_t before __attribute__((unused)), uint8_t after __attribute__((unused))) = 0;  

protected:
        SCQueue<KeyEvent*, KEYBOARD_QUEUE_CAPACITY> m_keyboard_events;  

};
