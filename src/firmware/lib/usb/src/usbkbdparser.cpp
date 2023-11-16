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
#include "usbkbdparser.h"
#include "bithacks.h"
#include "adbregisters.h"
#include "usb_hid_keys.h"

// #ifdef QUOKKADB
#include "platform_logmsg.h"
// #endif

extern bool global_debug;

KbdRptParser::KbdRptParser()
{
}

KbdRptParser::~KbdRptParser(){}

bool KbdRptParser::PendingKeyboardEvent()
{
    return !m_keyboard_events.isEmpty();
}

void KbdRptParser::Reset(void)
{
    while(PendingKeyboardEvent())
    {
        
        free(m_keyboard_events.dequeue());
    }

    // QuokkADB handles keyboard LEDs directly
    // ADBuino toggles LEDs status via keypress
    #ifdef QUOKKADB
    kbdLockingKeys.bLeds = 0x00;
    SetUSBkeyboardLEDs(false, false, false);
    #endif 
}

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
    MODIFIERKEYS mod;
    *((uint8_t *)&mod) = m;
    Logmsg.print((mod.bmLeftCtrl == 1) ? "C" : " ");
    Logmsg.print((mod.bmLeftShift == 1) ? "S" : " ");
    Logmsg.print((mod.bmLeftAlt == 1) ? "A" : " ");
    Logmsg.print((mod.bmLeftGUI == 1) ? "G" : " ");

    Logmsg.print(" >");
    #ifdef ADBIUNO
        PrintHex<uint8_t>(key, 0x80);
    #elif QUOKKADB
        Logmsg.print(key, fmtHEX);
    #endif
    Logmsg.print("< ");

    Logmsg.print((mod.bmRightCtrl == 1) ? "C" : " ");
    Logmsg.print((mod.bmRightShift == 1) ? "S" : " ");
    Logmsg.print((mod.bmRightAlt == 1) ? "A" : " ");
    Logmsg.println((mod.bmRightGUI == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{

    if (key == USB_KEY_NONE || key == USB_KEY_ERR_OVF)
    return;

    if (global_debug)
    {
        Logmsg.print("DN ");
        PrintKey(mod, key);
    }
    uint8_t c = OemToAscii(mod, key);

    if (c)
        OnKeyPressed(c);

    // Special handling of Caps Lock because of its held down nature
    if (key == USB_KEY_CAPSLOCK)
    {
        if (kbdLockingKeys.kbdLeds.bmCapsLock == 1)
        {
            B_UNSET(m_custom_mod_keys, CapsLockFlag);
            B_UNSET(m_custom_mod_keys, Led2CapsLockFlag);
            if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyDown, mod)))
            {
                Logmsg.println("Warning! unable to queue caps lock down");
            }
        }
        else
        {
            B_SET(m_custom_mod_keys, CapsLockFlag);
            B_SET(m_custom_mod_keys, Led2CapsLockFlag);
            if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyUp, mod)))
            {
                Logmsg.println("Warning! unable to queue caps lock up");
            }
        }        
    }
    else if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyDown, mod)))
    {
        Logmsg.println("Warning! unable to enqueue new KeyDown");
    }
    // If power button replacement queue key twice
    else if (key == USB_KEY_PAUSE || key == USB_KEY_F15)
    {
        if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyDown, mod)))
        {
            Logmsg.println("Warning! unable to enqueue new Power Button KeyDown");
        }
    }



    if (key == USB_KEY_NUMLOCK)
    {
        if (kbdLockingKeys.kbdLeds.bmNumLock == 1)
        {
            B_SET(m_custom_mod_keys, NumLockFlag);
            B_SET(m_custom_mod_keys, Led1NumLockFlag);
        }
        else
        {
            B_UNSET(m_custom_mod_keys, NumLockFlag);
            B_UNSET(m_custom_mod_keys, Led1NumLockFlag);
        }        
    }
    if (key == USB_KEY_SCROLLLOCK)
    {
        if (kbdLockingKeys.kbdLeds.bmScrollLock == 1)
        {
            B_SET(m_custom_mod_keys, ScrollLockFlag);
            B_SET(m_custom_mod_keys, Led3ScrollLockFlag);
        }
        else
        {
            B_UNSET(m_custom_mod_keys, ScrollLockFlag);
            B_UNSET(m_custom_mod_keys, Led3ScrollLockFlag);
        }        
    }

    if (key == USB_KEY_BACKSPACE)
    {
        B_SET(m_custom_mod_keys, DeleteFlag);
    }
}


void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
    if (key == USB_KEY_NONE || key == USB_KEY_ERR_OVF)
        return;

    if (global_debug)
    {
        Logmsg.print("UP ");
        PrintKey(mod, key);
    }
    if (key != USB_KEY_CAPSLOCK)
    {
        if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyUp, mod)))
        {
            Logmsg.println("Warning! unable to enqueue new KeyDown");
        }
    
        // If power button replacement queue key twice
        else if (key == USB_KEY_PAUSE || key == USB_KEY_F15)
        {
            if (!m_keyboard_events.enqueue(new KeyEvent(key, KeyEvent::KeyUp, mod)))
            {
                Logmsg.println("Warning! unable to enqueue new Power Button KeyUp");
            }
        }
        if (key == USB_KEY_BACKSPACE)
        {
            B_UNSET(m_custom_mod_keys, DeleteFlag);
        }
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
        Logmsg.print("Before: ");
        Logmsg.print(before,  fmtHEX);
        Logmsg.print(" after: ");
        Logmsg.print(after,  fmtHEX);
        Logmsg.print(" ");
        Logmsg.print(*((uint8_t *)&m_modifier_keys), fmtHEX);
    }
    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
    {
        if (global_debug)
        {
            Logmsg.println("LeftCtrl changed");
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
            Logmsg.println("LeftShift changed");
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
            Logmsg.println("LeftAlt changed");
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
            Logmsg.println("LeftGUI changed");
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
            Logmsg.println("RightCtrl changed");
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
            Logmsg.println("RightShift changed");
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
            Logmsg.println("RightAlt changed");
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
            Logmsg.println("RightGUI changed");
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

void KbdRptParser::OnKeyPressed(uint8_t key)
{
    if (global_debug)
    {

        Logmsg.print("ASCII: ");
        Logmsg.println((char)key);
    }
};
