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
