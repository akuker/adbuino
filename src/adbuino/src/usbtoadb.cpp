
#include "usb_hid_keys.h"
#include <stdint.h>

// Based upon the virtual keycodes provided at:
//    http://meandmark.com/keycodes.html

// Also refer to teh Guide to the Macintosh family hardware, figure 8-10:
//    https://archive.org/details/apple-guide-macintosh-family-hardware/page/n345/mode/2up


// Virtual Keycodes for the Mac QWERTY Layout
// Keycodes are in hexadecimal. A blank entry means either there is no key assigned to that keycode or I was unable to find the assigned key.

uint8_t usb_keycode_to_adb_code(uint8_t usb_code)
{
    switch (usb_code)
    {
    case USB_KEY_A:
        return 0x00;
    case USB_KEY_S:
        return 0x01;
    case USB_KEY_D:
        return 0x02;
    case USB_KEY_F:
        return 0x03;
    case USB_KEY_H:
        return 0x04;
    case USB_KEY_G:
        return 0x05;
    case USB_KEY_Z:
        return 0x06;
    case USB_KEY_X:
        return 0x07;
    case USB_KEY_C:
        return 0x08;
    case USB_KEY_V:
        return 0x09;
    case USB_KEY_B:
        return 0x0B;
    case USB_KEY_Q:
        return 0x0C;
    case USB_KEY_W:
        return 0x0D;
    case USB_KEY_E:
        return 0x0E;
    case USB_KEY_R:
        return 0x0F;
    case USB_KEY_Y:
        return 0x10;
    case USB_KEY_T:
        return 0x11;
    case USB_KEY_1:
        return 0x12;
    case USB_KEY_2:
        return 0x13;
    case USB_KEY_3:
        return 0x14;
    case USB_KEY_4:
        return 0x15;
    case USB_KEY_6:
        return 0x16;
    case USB_KEY_5:
        return 0x17;
    case USB_KEY_EQUAL:
        return 0x18;
    case USB_KEY_9:
        return 0x19;
    case USB_KEY_7:
        return 0x1A;
    case USB_KEY_MINUS:
        return 0x1B;
    case USB_KEY_8:
        return 0x1C;
    case USB_KEY_0:
        return 0x1D;
    case USB_KEY_RIGHTBRACE:
        return 0x1E;
    case USB_KEY_O:
        return 0x1F;
    case USB_KEY_U:
        return 0x20;
    case USB_KEY_LEFTBRACE:
        return 0x21;
    case USB_KEY_I:
        return 0x22;
    case USB_KEY_P:
        return 0x23;
    case USB_KEY_ENTER:
        return 0x24;
    case USB_KEY_L:
        return 0x25;
    case USB_KEY_J:
        return 0x26;
    case USB_KEY_APOSTROPHE:
        return 0x27;
    case USB_KEY_K:
        return 0x28;
    case USB_KEY_SEMICOLON:
        return 0x29;
    case USB_KEY_BACKSLASH:
        return 0x2A;
    case USB_KEY_COMMA:
        return 0x2B;
    case USB_KEY_SLASH:
        return 0x2C;
    case USB_KEY_N:
        return 0x2D;
    case USB_KEY_M:
        return 0x2E;
    case USB_KEY_DOT:
        return 0x2F;
    case USB_KEY_TAB:
        return 0x30;
    case USB_KEY_SPACE:
        return 0x31;
    case USB_KEY_GRAVE:
        return 0x32;
    case USB_KEY_BACKSPACE:
        return 0x33;
    case USB_KEY_ESC:
        return 0x35;
    case USB_KEY_KPDOT:
        return 0x41;
    case USB_KEY_KPASTERISK:
        return 0x43;
    case USB_KEY_KPPLUS:
        return 0x45;
    case USB_KEY_KPSLASH:
        return 0x4B;
    case USB_KEY_KPENTER:
        return 0x4C;
    case USB_KEY_KPMINUS:
        return 0x4E;
    case USB_KEY_KPEQUAL:
        return 0x51;
    case USB_KEY_KP0:
        return 0x52;
    case USB_KEY_KP1:
        return 0x53;
    case USB_KEY_KP2:
        return 0x54;
    case USB_KEY_KP3:
        return 0x55;
    case USB_KEY_KP4:
        return 0x56;
    case USB_KEY_KP5:
        return 0x57;
    case USB_KEY_KP6:
        return 0x58;
    case USB_KEY_KP7:
        return 0x59;
    case USB_KEY_KP8:
        return 0x5B;
    case USB_KEY_KP9:
        return 0x5C;
    case USB_KEY_F5:
        return 0x60;
    case USB_KEY_F6:
        return 0x61;
    case USB_KEY_F7:
        return 0x62;
    case USB_KEY_F3:
        return 0x63;
    case USB_KEY_F8:
        return 0x64;
    case USB_KEY_F9:
        return 0x65;
    case USB_KEY_F11:
        return 0x67;
    case USB_KEY_F13:
        return 0x69;
    case USB_KEY_F14:
        return 0x6B;
    case USB_KEY_F10:
        return 0x6D;
    case USB_KEY_F12:
        return 0x6F;
    case USB_KEY_F15:
        return 0x71;
    case USB_KEY_HELP:
        return 0x72;
    case USB_KEY_HOME:
        return 0x73;
    case USB_KEY_PAGEUP:
        return 0x74;
    case USB_KEY_DELETE:
        return 0x75;
    case USB_KEY_F4:
        return 0x76;
    case USB_KEY_END:
        return 0x77;
    case USB_KEY_F2:
        return 0x78;
    case USB_KEY_PAGEDOWN:
        return 0x79;
    case USB_KEY_F1:
        return 0x7A;
    case USB_KEY_LEFT:
        return 0x7B;
    case USB_KEY_RIGHT:
        return 0x7C;
    case USB_KEY_DOWN:
        return 0x7D;
    case USB_KEY_UP:
        return 0x7E;
    default:
        return 0x00;

        // 0x37        USB_KEY_Cmd (Apple)
        // 0x38        USB_KEY_LEFTSHIFT
        // 0x39        USB_KEY_CAPSLOCK
        // 0x3A        USB_KEY_Option
        // 0x3B        USB_KEY_Control
    }
}
// 0x00        USB_KEY_A
// 0x01        USB_KEY_S
// 0x02        USB_KEY_D
// 0x03        USB_KEY_F
// 0x04        USB_KEY_H
// 0x05        USB_KEY_G
// 0x06        USB_KEY_Z
// 0x07        USB_KEY_X
// 0x08        USB_KEY_C
// 0x09        USB_KEY_V
// 0x0B        USB_KEY_B
// 0x0C        USB_KEY_Q
// 0x0D        USB_KEY_W
// 0x0E        USB_KEY_E
// 0x0F        USB_KEY_R
// 0x10        USB_KEY_Y
// 0x11        USB_KEY_T
// 0x12        USB_KEY_1
// 0x13        USB_KEY_2
// 0x14        USB_KEY_3
// 0x15        USB_KEY_4
// 0x16        USB_KEY_6
// 0x17        USB_KEY_5
// 0x18        USB_KEY_EQUAL
// 0x19        USB_KEY_9
// 0x1A        USB_KEY_7
// 0x1B        USB_KEY_MINUS
// 0x1C        USB_KEY_8
// 0x1D        USB_KEY_0
// 0x1E        USB_KEY_RIGHTBRACE
// 0x1F        USB_KEY_O
// 0x20        USB_KEY_U
// 0x21        USB_KEY_LEFTBRACE
// 0x22        USB_KEY_I
// 0x23        USB_KEY_P
// 0x24        USB_KEY_ENTER
// 0x25        USB_KEY_L
// 0x26        USB_KEY_J
// 0x27        USB_KEY_APOSTROPHE
// 0x28        USB_KEY_K
// 0x29        USB_KEY_SEMICOLON
// 0x2A        USB_KEY_BACKSLASH
// 0x2B        USB_KEY_COMMA
// 0x2C        USB_KEY_SLASH
// 0x2D        USB_KEY_N
// 0x2E        USB_KEY_M
// 0x2F        USB_KEY_DOT
// 0x30        USB_KEY_TAB
// 0x31        USB_KEY_SPACE
// 0x32        USB_KEY_GRAVE
// 0x33        USB_KEY_BACKSPACE
// 0x35        USB_KEY_ESC
// // 0x37        USB_KEY_Cmd (Apple)
// // 0x38        USB_KEY_LEFTSHIFT
// // 0x39        USB_KEY_CAPSLOCK
// // 0x3A        USB_KEY_Option
// // 0x3B        USB_KEY_Control
// 0x41        USB_KEY_KPDOT
// 0x43        USB_KEY_KPASTERISK
// 0x45        USB_KEY_KPPLUS
// 0x4B        USB_KEY_KPSLASH
// 0x4C        USB_KEY_KPEQUAL
// 0x4E        USB_KEY_KPDOT
// 0x51        USB_KEY_KPEQUAL
// 0x52        USB_KEY_KP0
// 0x53        USB_KEY_KP1
// 0x54        USB_KEY_KP2
// 0x55        USB_KEY_KP3
// 0x56        USB_KEY_KP4
// 0x57        USB_KEY_KP5
// 0x58        USB_KEY_KP6
// 0x59        USB_KEY_KP7
// 0x5B        USB_KEY_KP8
// 0x5C        USB_KEY_KP9
// 0x60        USB_KEY_F5
// 0x61        USB_KEY_F6
// 0x62        USB_KEY_F7
// 0x63        USB_KEY_F3
// 0x64        USB_KEY_F8
// 0x65        USB_KEY_F9
// 0x67        USB_KEY_F11
// 0x69        USB_KEY_F13
// 0x6B        USB_KEY_F14
// 0x6D        USB_KEY_F10
// 0x6F        USB_KEY_F12
// 0x71        USB_KEY_F15
// 0x72        USB_KEY_HELP
// 0x73        USB_KEY_HOME
// 0x74        USB_KEY_PAGEUP
// 0x75        USB_KEY_DELETE
// 0x76        USB_KEY_F4
// 0x77        USB_KEY_END
// 0x78        USB_KEY_F2
// 0x79        USB_KEY_PAGEDOWN
// 0x7A        USB_KEY_F1
// 0x7B        USB_KEY_LEFT
// 0x7C        USB_KEY_RIGHT
// 0x7D        USB_KEY_DOWN
// 0x7E        USB_KEY_UP
