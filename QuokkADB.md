# QuokkADB
A USB keyboard and mouse to ADB (Apple Desktop Bus) keyboard and mouse emulator.
The firmware is based upon the ADBuino and is in a unified repository [https://github.com/akuker/adbuino](https://github.com/akuker/adbuino).

## Current Status
 - Caps Lock with keyboard LED caps lock indicator working
 - Normal Apple mouse emulation working (extended mouse not implemented)
 - Normal Apple US keyboard emulation working  (extended keyboard and international keyboard not implemented)
 - Logitech Unifying receiver works
 - Rear facing and front facing ADB can be connected to host (Mac)
 or used to connected to another ADB device
 - Mouse and keyboard readdressing not implemented (meaning if a USB mouse or keyboard is a attached along with the ADB version of the device, the devices may or may not work)
 - "Listen" ADB protocols not implemented
 - Rewriting handler ID value not implemented, because of the "Listen" protocol not being implemented which in turn means the keyboard and mouse can not be used as extended devices
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

