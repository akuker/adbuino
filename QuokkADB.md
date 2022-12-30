# QuokkADB
A USB keyboard and mouse to ADB (Apple Desktop Bus) keyboard and mouse emulator.
The firmware is based upon the ADBuino and is in a unified repository [https://github.com/akuker/adbuino](https://github.com/akuker/adbuino).

## Current Status
 - Caps Lock with keyboard LED caps lock indicator working
 - Normal Apple mouse emulation working
 - Extended Apple US keyboard emulation working - left and right modifier keys when the OS switches keyboard type
 - Logitech Unifying receiver works
 - Scroll Lock, Pause/Break as F14, and F15 respectively 
 - Rear facing and front facing ADB can be connected to host (Mac)
 or used to connected to another ADB device
 - Mouse and keyboard readdressing implemented so multiple QuokkADBs, other ADB keyboards, and/or mice can be used 
 - Multiple USB keyboards or mice can be attached and will work. Known issue: Caps lock led is limited to changing on the keyboard which it was pressed.
 - Limitations with USB Host support in TinyUSB mean only one level of USB hubs may be attached and some USB composite keyboard have been found not to work.

## Parity with USB Wombat
 - Context menu for Mac OS 8.1 and above - middle mouse button switches right click mode
   - default: right click actives Ctrl+left click
   - other:  right click is sent over the ADB bus as a right click (should work on a NeXT box)
 - SysRq/Print Screen and F13 work as the Apple Power key (keycode 0x7F, separate from ADB power on button on the QuokkADB)
 - Ctrl + Shift + CapsLock + V - ghost types the firmware version to the host computer
 - power button on QuokkADB will boot computer while QuokkADB is powered off
  
## QuokkADB LED blink meaning
 - On boot - one blink
 - On keyboard mount (connected to usb) - two blinks
 - On mouse mount (connected to usb)- three blinks
 - On device umount (disconnected to usb) - one blink
 - Normal operation - very dim light when idle, blinks when there is activity on the ADB bus for the device.

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

## Known Bugs
 - Pressing buttons that change the LEDs on the keyboard (Caps lock, num lock, scroll lock) stop changing the LEDs after a few LED changes. Besides the LEDs not changing, the keyboard works normally.