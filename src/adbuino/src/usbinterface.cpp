//---------------------------------------------------------------------------
//
//	ADBuino ADB keyboard and mouse adapter
//
//	   Copyright (C) 2021-2022 akuker
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
//---------------------------------------------------------------------------

#include "usbinterface.h"

void UsbInterface::Init(){
    Usb = new USB();
    // Hub1 = new USBHub(Usb);
    // Hub2 = new USBHub(Usb);
    // Hub3 = new USBHub(Usb);
    // Hub4 = new USBHub(Usb);
    // HidKeyboard = new HIDBoot<USB_HID_PROTOCOL_KEYBOARD>(Usb);
    // HidMouse = new HIDBoot<USB_HID_PROTOCOL_MOUSE>(Usb);
    // MousePrs = new MouseRptParser();
    // KeyboardPrs = new KbdRptParser();

    if (Usb->Init() == -1)
        Serial.println("OSC did not start.");
    delay(200);

    // HidKeyboard->SetReportParser(0, KeyboardPrs);
    // HidMouse->SetReportParser(0, MousePrs);

    // next_time = (uint32_t)millis() + 10000;
    Serial.println("Done init USB");
}

void UsbInterface::RunTask(){

    Usb->Task();
}
