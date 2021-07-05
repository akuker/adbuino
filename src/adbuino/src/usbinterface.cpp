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
