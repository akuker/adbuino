#include <hidcomposite.h>
#include <usbhub.h>
#include <hidboot.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


// Override HIDComposite to be able to select which interface we want to hook into
class MyHIDSelector : public HIDComposite
{
public:
    MyHIDSelector(USB *p) : HIDComposite(p) {};

protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);
};


bool MyHIDSelector::SelectInterface(uint8_t iface, uint8_t proto)
{
    return true;
}


// Will be called for all HID data received from the USB interface
void MyHIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    char mystring[256];
    if (len && buf)  {
        Serial.print(F("\r\nEp"));
        Serial.print(ep);
        Serial.print(" sizeof(MOUSEINFO):");
        Serial.print(sizeof(MOUSEINFO));
        Serial.print(" sizeof(KBDINFO):");
        Serial.print(sizeof(KBDINFO));
        Serial.print(" len:");
        Serial.print(len);
        
        Serial.print(" -- ");
        for (uint8_t i = 0; i < len; i++) {
            if (buf[i] < 16) Serial.print(F("0"));
            Serial.print(buf[i], HEX);
            Serial.print(F(" "));
        }
    }
    if(len == sizeof(MOUSEINFO)){

        Serial.println("Mouse!");
        MOUSEINFO *mi = (MOUSEINFO*)buf;
        sprintf(mystring,"Left:%02X Right:%02X Mid:%02X DX:%d DY:%d\n\r",
            mi->bmLeftButton, mi->bmRightButton, mi->bmMiddleButton, mi->dX, mi->dY);
        Serial.println(mystring);
    }

    if(len == sizeof(KBDINFO)){

        Serial.println("Keyboard!");
        // KBDINFO *ki = (KBDINFO*)buf;
        // sprintf(mystring,"Left:%02X Right:%02X Mid:%02X DX:%d DY:%d\n\r",
        //     mi->bmLeftButton, mi->bmRightButton, mi->bmMiddleButton, mi->dX, mi->dY);
        // Serial.println(mystring);
    }


}


USB         Usb;
USBHub      Hub(&Usb);
USBHub      Hub1(&Usb);
USBHub      Hub2(&Usb);
USBHub      Hub3(&Usb);
MyHIDSelector hidSelector(&Usb);
MyHIDSelector hidSelector2(&Usb);

void setup()
{
    Serial.begin( 115200 );
#if !defined(__MIPSEL__)
    while (!Serial);
#endif
    Serial.println("Start");
  // Clear the reset signal from the USB controller
  pinMode(7, OUTPUT);
  digitalWrite(7,HIGH);

    // Setup blinking LED
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");
    else 
        Serial.println("OSC started");

    delay( 200 );

//   hidSelector.SetReportParser(0, &KeyboardPrs);
//   hidSelector.SetReportParser(1, &MousePrs);

    Serial.println("setup complete");
}

static uint8_t prev_state = 0;

void loop()
{
    Usb.Task();

    // USB state
    if (prev_state != Usb.getUsbTaskState()) {
        prev_state = Usb.getUsbTaskState();
        Serial.print(F("usb_state: "));
        Serial.println(Usb.getUsbTaskState(), HEX);
    }
}