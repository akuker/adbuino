# QuokkADB
A USB keyboard and mouse to ADB (Apple Desktop Bus) keyboard and mouse emulator.
The firmware is based upon the ADBuino and is in a unified repository [https://github.com/akuker/adbuino](https://github.com/akuker/adbuino).

## Current Status
 - Caps Lock with keyboard LED caps lock indicator working
 - Normal Apple mouse emulation working (extended mouse not implemented)
 - Normal Apple US keyboard emulation working  (extended keyboard and international keyboard not implemented)
 - Logitech Unifying receiver works
 - Right mouse click works on Mac OSes that support it
 - Pause/Break and F15 work as the Apple Power key (keycode 0x7F, separate from ADB power on button on the QuokkADB)
 - SysRq/Print Screen, Scroll Lock as F13, and F14 respectively 
 - Rear facing and front facing ADB can be connected to host (Mac)
 or used to connected to another ADB device
 - Mouse and keyboard readdressing implemented so multiple QuokkADBs other ADB keyboards and/or mice can be used also 
 - Multiple USB keyboards or mice can be attached and will work. Known issue: Caps lock led is limited to changing on the keyboard which it was pressed.
 - Limitations with USB Host support in TinyUSB mean only one level of USB hubs may be attached and some USB composite keyboard have been found not to work.

## QuokkADB LED blink meaning
 - On boot - one blink
 - On keyboard mount (connected to usb) - two blinks
 - On mouse mount (connected to usb)- three blinks
 - On device umount (disconnected to usb) - one blink
 - Normal operation - LED off to save current draw 

### Normal LED sequence:
If both a mouse and keyboard are connected to the QuokkADB.

Turning on the Mac:
 1. 1 blink - power on
 2. 2 blinks - keyboard mounted
 3. 3 blinks - mouse mounted

Removing mouse:
 1. 1 blink

Inserting a new mouse:
 1. 3 blinks

