# QuokkADB
A USB keyboard and mouse to ADB (Apple Desktop Bus) keyboard and mouse emulator.
The firmware is based upon the ADBuino and is in a unified repository [https://github.com/akuker/adbuino](https://github.com/akuker/adbuino).

## How to use
1. Make sure the QuokkADB is plugged into the ADB chain and
the ADB chain is connected to the host computer before powering it up (the host computer writes settings to the QuokkADB at boot and does not rewrite settings on reconnect. On the same note, do not unplug the QuokkADB from the ADB chain while the computer is on as the QuokkADB will lose its settings).
1. The button on the QuokkADB will turn on most computers (otherwise use the switch on the computer).
2. Once the power is turned on LEDs on the QuokkADB will blink once for power and then blink for all devices found on the USB bus.
3. The QuokkADB LED should blink rapidly and brightly whenever the QuokkADB is reading or writing to the ADB bus. It should be very dim otherwise.


## The board
### Ports
 - USB port: plug in your keyboard, mouse, or hub here.
 - Front and Back ADB ports: either port can be directly hooked up to the host computer or connected to a ADB chain. One port should be connected towards the host while the other may be connected to other ADB devices.
 - QuokkADB stack port: This port uses a 4 pin JST to JST wire to stack two QuokkADBs together giving two USB ports. The [Qwiic](https://www.sparkfun.com/categories/tags/qwiic-cables). JST cable can be used to connect two QuokkADBs together.
The stack port carries all four ADB signals.
    - pin 1 ADB
    - pin 2 Power Switch
    - pin 3 GND
    - pin 4 +5V ADB power


![QuokkADB picture](images/quokkadb.jpg)

## Special Key-combos
The special keyboad commands start with `CTRL + SHIFT + CAPLOCK` and a letter key.  Example: `CTRL + SHIFT + CAPLOCK + V` ghost types the firmware version.
 - `V` - Ghost types firmware version
 - `L` - Ghost types the busy LED status (ON/OFF) and saves the new status to flash and will be used when the board powers on again. This controls
 whether the LED blinks when accessing the ADB bus or remains off for light sensitive situations.

## Programming 
The firmware version can be found by opening up a empty text document or program that allows typing. 
Press `Ctrl + Shift + CapsLock + V` and the firmware version will be ghost typed.
To change the firmware do the following:
1. Disconnect the ADB ports, and the QuokkADB stack port on the QuokkADB
2. Short the two boot loader contacts labeled "BOOTLDR
![Boot loader pins](images/quokkadb-bootloader.jpg)
3. Connect a USB A to USB A cable to the QuokkADB USB port and
the computer with the `.uf2` file on it. Firmware can be downloaded from [here](https://github.com/rabbitholecomputing/QuokkADB-firmware/releases).
1. Once connected a mass storage device drive should be mounted on the computer (at this point the boot loader contacts can be 
disconnected).  
1. Copy the `.uf2` to the new drive
2. Once the file has finished copying one of two things should happen
     1. If the boot loader is still shorted, the mass storage device drive will mount again but without the firmware file on the drive. 
     2. If the boot loader is not shorted anymore, the QuokkADB will blink once then remain very dim.

     In either case the new firmware is installed and once the
     boot loader contacts are no longer shorted the board is ready to use.

## Current Status
 - Caps Lock with keyboard LED caps lock indicator working
 - Normal Apple mouse emulation working
 - Extended Apple US keyboard emulation working - left and right modifier keys when the OS switches keyboard type
 - Logitech Unifying receiver works
 - Scroll Lock, Pause/Break as F14, and F15 respectively 
 - Rear facing and front facing ADB can be connected to host (Mac)
 or used to connected to another ADB device
 - Mouse and keyboard readdressing implemented so multiple QuokkADBs, other ADB keyboards, and/or mice can be used 
 - Multiple USB keyboards or mice can be attached and will work.

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
 - On device unmount (disconnected to usb) - one blink
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
 - Pressing buttons that change the LEDs on the keyboard (Caps lock, num lock, scroll lock) stops changing the LEDs after many LED toggles. The keyboard functions normally and functions like caps lock still work but its LED status no longer changes.
 - Some USB hubs do not seem to work