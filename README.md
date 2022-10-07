# Background

This is a fork of Difegue's version of the [adbuino](https://github.com/Difegue/Chaotic-Realm), which was a modified version of [bbraun's](http://synack.net/svn/adbduino/) PS/2 to ADB arduino sketch, with some extra code added to alleviate issues with his own PS/2 keyboard.  For Difegue's original write-up, please read more info [here.](https://tvc-16.science/adbuino-ps2.html).

![ADBuino picture](images/adbuino_0p9.png)

# QuokkADB

QuokkADB is a modified version of [adbuino](https://github.com/akuker/adbuino). It is targeted to the Rabbit Hole Computing's QuokkADB device. QuokkADB is a Raspberry Pi RP2040 based device that takes in USB inputs from a keyboard and/or mouse and outputs Apple's ADB (Apple Desktop Bus).  

# Hack-y Alternative
The USB ADBuino is essentially a "value-engineered" version of a bunch of off-the-shelf components. 

<a href="https://www.sparkfun.com/products/11021"><img src="images/arduino_uno.jpg" width="150"/></a> <a href="https://www.sparkfun.com/products/9947"><img src="images/host_shield.jpg" width="150"/></a> 
<a href="https://www.sparkfun.com/products/12009"><img src="images/logic_level_converter.jpg" width="150"/></a>

<img src="images/hacky_adbuino.png" width="600"/>

# How to build and flash an adbuino

Note: This software is intended to be compiled in an Ubuntu Linux environment.

Any dependencies that are configured in the PlatformIO project (such as TinyUSB) will automatically be downloaded. Make sure you are connected to the Internet the first time you build this software.

- Install [Visual Studio Code](https://code.visualstudio.com/)
- Install [Platform IO](https://platformio.org/install)
- Open Visual studio Code and open the directory ./src/adbuino
- Select the PlatformIO icon on the left
- Execute the `Build` project task
- Execute the `Upload` project task to flash your device

# What's this repo for?

The intent of this repository is to add "USB Host" support to the original ADB design. This will allow a USB keyboard and/or mouse to be used to drive an ADB host, as well as PS/2. 

The [BMOW "Wombat"](https://www.bigmessowires.com/usb-wombat/) is a closed-source alternative to adbuino. The [BMOW "Wombat"](https://www.bigmessowires.com/usb-wombat/) is an AWESOME project, but doesn't allow external contributors. 

# References
![ADB Pinout](images/adb_pinout.png)

[TinyUSB Library](https://github.com/raspberrypi/tinyusb)

[Running OpenOCD without root](https://forgge.github.io/theCore/guides/running-openocd-without-sudo.html)

[Apple ADB Manager Documenation](https://developer.apple.com/library/archive/documentation/mac/pdf/Devices/ADB_Manager.pdf)

https://www.mouser.com/ProductDetail/TE-Connectivity/5749181-1?qs=XlZqES4cpWbRcAMR%2FcJqkQ%3D%3D

[MiSTER adb hardware emulation](https://github.com/mist-devel/plus_too/blob/master/adb.v)
