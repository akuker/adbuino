/**
 * USB2ADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 * This file is derived from adbuino:  https://github.com/akuker/adbuino
 * see main.cpp for license information
 * 
 * 26 September 2022 - Modifed by Rabbit Hole Computing LLC
 */


#include "usbkbdparser.h"
#include "bithacks.h"
#include "adbregisters.h"
#include "usb_hid_keys.h"
#include "tusb.h"

KbdRptParser::KbdRptParser()
{
    // maybe init something later
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
    return kbdreg2;
}


void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyDown)))
    {
        // failed to add to queue
    }

    if (key == USB_KEY_SCROLLLOCK)
    {
        B_UNSET(m_custom_mod_keys, ScrollLockFlag);
        if (B_IS_SET(m_custom_mod_keys, Led3ScrollLockFlag))
        {
            B_TOGGLE(m_custom_mod_keys, Led3ScrollLockFlag);
        }
    }
    if (key == USB_KEY_CAPSLOCK)
    {
        B_UNSET(m_custom_mod_keys, CapsLockFlag);
        if (B_IS_SET(m_custom_mod_keys, Led2CapsLockFlag))
        {
            B_UNSET(m_custom_mod_keys, Led2CapsLockFlag);
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

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
    {
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
        if (afterMod.bmLeftGUI)
        {
            OnKeyDown(0, USB_KEY_LEFTMETA);
        }
        else
        {
            OnKeyUp(0, USB_KEY_LEFTMETA);
        }
    }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{


    if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyUp)))
    {
        // Queue full
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