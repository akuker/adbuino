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
#include <Arduino.h>
#include "adbkbdparser.h"
#include "bithacks.h"
#include "adbregisters.h"
#include "usb_hid_keys.h"

#ifdef QUOKKADB
// #include "platform_logmsg.h"
//extern rp2040_serial::RPSerial Logmsg;
#endif
#include <platform_logmsg.h>

extern bool global_debug;

ADBKbdRptParser::ADBKbdRptParser()
{
}

ADBKbdRptParser::~ADBKbdRptParser()
{

}

uint16_t ADBKbdRptParser::GetAdbRegister0()
{
    KeyEvent *event;
    uint16_t kbdreg0 = 0;
    uint8_t adb_keycode = 0;
    bool isKeyUp;
    // Pack the first key event
    if (!m_keyboard_events.isEmpty())
    {
        event = m_keyboard_events.dequeue();
        isKeyUp = event->IsKeyUp();
        if (isKeyUp)
        {
            B_SET(kbdreg0, ADB_REG_0_KEY_1_STATUS_BIT);
        }
        adb_keycode = usb_keycode_to_adb_code(event->GetKeycode());
        kbdreg0 |= (adb_keycode << ADB_REG_0_KEY_1_KEY_CODE);
        delete(event);
    }
    else
    {
        kbdreg0 |= (ADB_REG_0_NO_KEY << ADB_REG_0_KEY_1_KEY_CODE);
    }

    if (adb_keycode == ADB_POWER_KEYCODE) {
        if (isKeyUp)
        {
            B_SET(kbdreg0, ADB_REG_0_KEY_1_STATUS_BIT);
        }
        kbdreg0 |= (adb_keycode << ADB_REG_0_KEY_2_KEY_CODE);
    }
    else{
        if (!m_keyboard_events.isEmpty()) {
            event = m_keyboard_events.peek();
            // if the first key wasn't the power key but the second one is, skip the second key packing
            // so on the next cycle the power key will be packed as both the first and second key
            if (event != NULL && usb_keycode_to_adb_code(event->GetKeycode()) != ADB_POWER_KEYCODE)
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
        }
        else   {
            kbdreg0 |= (ADB_REG_0_NO_KEY << ADB_REG_0_KEY_2_KEY_CODE);
        }
    }
    if (global_debug)
    {

        Logmsg.print("Keyboard Register 0 = ");
        Logmsg.println(kbdreg0, fmtHEX);
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

uint16_t ADBKbdRptParser::GetAdbRegister2()
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
        Logmsg.print("Kbdreg2 is ");
        Logmsg.println(kbdreg2, fmtHEX);
    }
    return kbdreg2;
}
