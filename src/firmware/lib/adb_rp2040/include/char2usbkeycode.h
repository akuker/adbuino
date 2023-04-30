#include <stdint.h>


struct usbkey_t
{
    uint8_t keycode;
    bool shift_down;
};


usbkey_t char_to_usb_keycode(char character);