//----------------------------------------------------------------------------
//
//  ADBuino ADB keyboard and mouse adapter
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2007 Peter H Anderson
//
//  This file is part of ADBuino.
//
//  ADBuino is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
// any later version.
//
//  ADBuino is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with ADBuino. If not, see <https://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------

#pragma once

#include "Arduino.h"

// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>
#include "adbmouseparser.h"
#include "adbkbdparser.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class UsbInterface
{
public:
    void Init();
    void RunTask();
    void PrintAllAddresses(UsbDevice *pdev);
    void PrintAddress(uint8_t addr);
    void PrintDescriptors(uint8_t addr);

    void PrintAllDescriptors(UsbDevice *pdev);

    void printhubdescr(uint8_t *descrptr, uint8_t addr);
    uint8_t getconfdescr(uint8_t addr, uint8_t conf);
    // function to get all string descriptors
    uint8_t getallstrdescr(uint8_t addr);
    //  function to get single string description
    uint8_t getstrdescr(uint8_t addr, uint8_t idx);

    /* prints hex numbers with leading zeroes */
    // copyright, Peter H Anderson, Baltimore, MD, Nov, '07
    // source: http://www.phanderson.com/arduino/arduino_display.html
    void print_hex(int v, int num_places);

    /* function to print configuration descriptor */
    void printconfdescr(uint8_t *descr_ptr);

    /* function to print interface descriptor */
    void printintfdescr(uint8_t *descr_ptr);

    /* function to print endpoint descriptor */
    void printepdescr(uint8_t *descr_ptr);

    /*function to print unknown descriptor */
    void printunkdescr(uint8_t *descr_ptr);

    /* Print a string from Program Memory directly to save RAM */
    void printProgStr(const char *str);

    uint8_t getdevdescr(uint8_t addr, uint8_t &num_conf);

private:
    USB *Usb;
    USBHub *Hub1;
    USBHub *Hub2;
    USBHub *Hub3;
    USBHub *Hub4;
    HIDBoot<USB_HID_PROTOCOL_KEYBOARD> *HidKeyboard;
    HIDBoot<USB_HID_PROTOCOL_MOUSE> *HidMouse;

    ADBMouseRptParser *MousePrs;
    ADBKbdRptParser *KeyboardPrs;
    uint32_t next_time;
};