//---------------------------------------------------------------------------
//
//	ADBuino & QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the ADBuino and the QuokkaADB projects.
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
//----------------------------------------------------------------------------

#include "usbkbdparser.h"
#include "bithacks.h"
#include "adbregisters.h"
#include "usb_hid_keys.h"

#ifdef QUOKKADB
#include "rp2040_serial.h"
using rp2040_serial::Serial;
#endif

extern bool global_debug;

KbdRptParser::KbdRptParser()
{
    if (global_debug)
    {
        // Disabled println - prints before uart is setup for QuokkADB
        // Serial.println("Running KbdRptParser::KbdRptParser()");
    }
}

bool KbdRptParser::PendingKeyboardEvent()
{
    return !m_keyboard_events.isEmpty();
}

uint16_t KbdRptParser::GetAdbRegister0()
{
    KeyEvent *event;
    uint16_t kbdreg0 = 0;
    uint8_t adb_keycode = 0;

    // Pack the first key event
    if (!m_keyboard_events.isEmpty())
    {
        event = m_keyboard_events.dequeue();
        if (event->IsKeyUp())
        {
            B_SET(kbdreg0, ADB_REG_0_KEY_1_STATUS_BIT);
        }
        adb_keycode = usb_keycode_to_adb_code(event->GetKeycode());
        kbdreg0 |= (adb_keycode << ADB_REG_0_KEY_1_KEY_CODE);
        free(event);
    }
    else
    {
        kbdreg0 |= (ADB_REG_0_NO_KEY << ADB_REG_0_KEY_1_KEY_CODE);
    }

    // Pack the second key event
    if (!m_keyboard_events.isEmpty())
    {
        event = m_keyboard_events.dequeue();
        if (event->IsKeyUp())
        {
            B_SET(kbdreg0, ADB_REG_0_KEY_2_STATUS_BIT);
        }
        adb_keycode = usb_keycode_to_adb_code(event->GetKeycode());
        kbdreg0 |= (adb_keycode << ADB_REG_0_KEY_2_KEY_CODE);
        free(event);
    }
    else
    {
        kbdreg0 |= (ADB_REG_0_NO_KEY << ADB_REG_0_KEY_2_KEY_CODE);
    }
    if (global_debug)
    {

        Serial.print("Keyboard Register 0 = ");
        Serial.println(kbdreg0, HEX);
    }
    return kbdreg0;
}

// Bit   Meaning
// 15  = None (reserved)
// 14  = Delete
// 13  = Caps Lock
// 12  = Reset
// 11  = Control
// 10  = Shift
// 9   = Option
// 8   = Command
// 7   = Num Lock/Clear
// 6   = Scroll Lock
// 5-3 = None (reserved)
// 2   = LED 3 (Scroll Lock)
// 1   = LED 2 (Caps Lock)
// 0   = LED 1 (Num Lock)

uint16_t KbdRptParser::GetAdbRegister2()
{
    uint16_t kbdreg2 = ADB_REG_2_DEFAULT;
    if (B_IS_SET(m_custom_mod_keys, DeleteFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_DELETE);
    }
    if (B_IS_SET(m_custom_mod_keys, CapsLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_CAPS_LOCK);
    }
    // Reset not implemented - this is only for Apple II
    if (m_modifier_keys.bmLeftCtrl || m_modifier_keys.bmRightCtrl)
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_CONTROL);
    }
    if (m_modifier_keys.bmLeftShift || m_modifier_keys.bmRightShift)
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_SHIFT);
    }
    if (m_modifier_keys.bmLeftAlt || m_modifier_keys.bmRightAlt)
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_OPTION);
    }
    if (m_modifier_keys.bmLeftGUI || m_modifier_keys.bmRightGUI)
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_COMMAND);
    }
    if (B_IS_SET(m_custom_mod_keys, NumLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_NUM_LOCK);
    }
    if (B_IS_SET(m_custom_mod_keys, ScrollLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_SCROLL_LOCK);
    }
    if (B_IS_SET(m_custom_mod_keys, Led3ScrollLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_SCROLL_LOCK_LED);
    }
    if (B_IS_SET(m_custom_mod_keys, Led2CapsLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_CAPS_LOCK_LED);
    }
    if (B_IS_SET(m_custom_mod_keys, Led1NumLockFlag))
    {
        B_UNSET(kbdreg2, ADB_REG_2_FLAG_NUM_LOCK_LED);
    }
    if (global_debug)
    {
        Serial.print("Kbdreg2 is ");
        Serial.println(kbdreg2, HEX);
    }
    return kbdreg2;
}

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
    MODIFIERKEYS mod;
    *((uint8_t *)&mod) = m;
    Serial.print((mod.bmLeftCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI == 1) ? "G" : " ");

    Serial.print(" >");
    #ifdef ADBIUNO
        PrintHex<uint8_t>(key, 0x80);
    #elif QUOKKADB
        Serial.print(key, HEX);
    #endif
    Serial.print("< ");

    Serial.print((mod.bmRightCtrl == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    if (global_debug)
    {
        Serial.print("DN ");
        PrintKey(mod, key);
    }
    uint8_t c = OemToAscii(mod, key);
    // m_last_key_pressed = key;
    // m_last_key_up_or_down = KeyEvent::KeyDown;

    if (c)
        OnKeyPressed(c);


    if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyDown)))
    {
        Serial.println("Warning! unable to enqueue new KeyDown");
    }

    if (key == USB_KEY_SCROLLLOCK)
    {
        B_UNSET(m_custom_mod_keys, ScrollLockFlag);
        if (B_IS_SET(m_custom_mod_keys, Led3ScrollLockFlag))
        {
            B_TOGGLE(m_custom_mod_keys, Led3ScrollLockFlag);
        }
    }
    if (key == USB_KEY_NUMLOCK)
    {
        B_UNSET(m_custom_mod_keys, NumLockFlag);
        if (B_IS_SET(m_custom_mod_keys, Led1NumLockFlag))
        {
            B_UNSET(m_custom_mod_keys, Led1NumLockFlag);
        }
    }
    if (key == USB_KEY_DELETE)
    {
        B_UNSET(m_custom_mod_keys, DeleteFlag);
    }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

    MODIFIERKEYS beforeMod;
    *((uint8_t *)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t *)&afterMod) = after;

    m_modifier_keys = *((MODIFIERKEYS *)&after);

    if (global_debug)
    {
        Serial.print("Before: ");
        Serial.print(before, HEX);
        Serial.print(" after: ");
        Serial.print(after, HEX);
        Serial.print(" ");
        Serial.print(*((uint8_t *)&m_modifier_keys), HEX);
    }
    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
    {
        if (global_debug)
        {
            Serial.println("LeftCtrl changed");
        }
        if (afterMod.bmLeftCtrl)
        {
            OnKeyDown(0, USB_KEY_LEFTCTRL);
        }
        else
        {
            OnKeyUp(0, USB_KEY_LEFTCTRL);
        }
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift)
    {
        if (global_debug)
        {
            Serial.println("LeftShift changed");
        }
        if (afterMod.bmLeftShift)
        {
            OnKeyDown(0, USB_KEY_LEFTSHIFT);
        }
        else
        {
            OnKeyUp(0, USB_KEY_LEFTSHIFT);
        }
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt)
    {
        if (global_debug)
        {
            Serial.println("LeftAlt changed");
        }
        if (afterMod.bmLeftAlt)
        {
            OnKeyDown(0, USB_KEY_LEFTALT);
        }
        else
        {
            OnKeyUp(0, USB_KEY_LEFTALT);
        }
    }
    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI)
    {
        if (global_debug)
        {
            Serial.println("LeftGUI changed");
        }
        if (afterMod.bmLeftGUI)
        {
            OnKeyDown(0, USB_KEY_LEFTMETA);
        }
        else
        {
            OnKeyUp(0, USB_KEY_LEFTMETA);
        }
    }

    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl)
    {
        if (global_debug)
        {
            Serial.println("RightCtrl changed");
        }
        if (afterMod.bmRightCtrl)
        {
            OnKeyDown(0, USB_KEY_RIGHTCTRL);
        }
        else
        {
            OnKeyUp(0, USB_KEY_RIGHTCTRL);
        }
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift)
    {
        if (global_debug)
        {
            Serial.println("RightShift changed");
        }
        if (afterMod.bmRightShift)
        {
            OnKeyDown(0, USB_KEY_RIGHTSHIFT);
        }
        else
        {
            OnKeyUp(0, USB_KEY_RIGHTSHIFT);
        }
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt)
    {
        if (global_debug)
        {
            Serial.println("RightAlt changed");
        }
        if (afterMod.bmRightAlt)
        {
            OnKeyDown(0, USB_KEY_RIGHTALT);
        }
        else
        {
            OnKeyUp(0, USB_KEY_RIGHTALT);
        }
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI)
    {
        if (global_debug)
        {
            Serial.println("RightGUI changed");
        }
        if (afterMod.bmRightGUI)
        {
            OnKeyDown(0, USB_KEY_RIGHTMETA);
        }
        else
        {
            OnKeyUp(0, USB_KEY_RIGHTMETA);
        }
    }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
    if (global_debug)
    {
        Serial.print("UP ");
        PrintKey(mod, key);
    }

    if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyUp)))
    {
        Serial.println("Warning! unable to enqueue new KeyDown");
    }

    if (key == USB_KEY_SCROLLLOCK)
    {
        B_SET(m_custom_mod_keys, ScrollLockFlag);
        if (!B_IS_SET(m_custom_mod_keys, Led3ScrollLockFlag))
        {
            B_SET(m_custom_mod_keys, Led3ScrollLockFlag);
        }
    }
    if (key == USB_KEY_CAPSLOCK)
    {
        B_SET(m_custom_mod_keys, CapsLockFlag);
        if (!B_IS_SET(m_custom_mod_keys, Led2CapsLockFlag))
        {
            B_SET(m_custom_mod_keys, Led2CapsLockFlag);
        }
    }
    if (key == USB_KEY_NUMLOCK)
    {
        B_SET(m_custom_mod_keys, NumLockFlag);
        if (!B_IS_SET(m_custom_mod_keys, Led1NumLockFlag))
        {
            B_SET(m_custom_mod_keys, Led1NumLockFlag);
        }
    }
    if (key == USB_KEY_DELETE)
    {
        B_SET(m_custom_mod_keys, DeleteFlag);
    }
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
    if (global_debug)
    {

        Serial.print("ASCII: ");
        Serial.println((char)key);
    }
};
