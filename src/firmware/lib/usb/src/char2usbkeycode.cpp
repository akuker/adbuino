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
//  with the file. If not, see <https://www.gnu.org/licenses/>.
//
//----------------------------------------------------------------------------

#include "usb_hid_keys.h"
#include "char2usbkeycode.h"

usbkey_t char_to_usb_keycode(char character)
{
    usbkey_t key;
    if (character >= 'A' && character <= 'Z')
    {
        key.keycode = USB_KEY_A + (character - 'A');
        key.shift_down = true;
        return key;
    }

    if (character >= 'a' && character <= 'z')
    {
        key.keycode = USB_KEY_A + (character - 'a');
        key.shift_down = false;
        return key;
    }

    if (character == '.')
    {
        key.keycode = USB_KEY_DOT;
        key.shift_down = false;
        return key;
    }

    if (character == '0')
    {
        key.keycode = USB_KEY_0;
        key.shift_down = false;
        return key;
    }

    if (character >= '1' && character <= '9')
    {
        key.keycode = USB_KEY_1 + (character - '1');
        key.shift_down = false;
        return key;
    }

    if (character == ':')
    {
        key.keycode = USB_KEY_SEMICOLON;
        key.shift_down = true;
        return key;
    }

    if (character == ' ')
    {
        key.keycode = USB_KEY_SPACE;
        key.shift_down = false;
        return key;
    }

    if (character == '-')
    {
        key.keycode = USB_KEY_MINUS;
        key.shift_down = false;
        return key;
    }


    if (character == '\n')
    {
        key.keycode = USB_KEY_ENTER;
        key.shift_down = true;
        return key;
    }

    key.keycode = 0;
    key.shift_down = false;
    return key;
}
