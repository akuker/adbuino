#include <hidcomposite.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <hidboot.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include "usbhid.h"


struct ABSOLUTEMOUSEINFO {

        struct {
                uint8_t bmLeftButton : 1;
                uint8_t bmRightButton : 1;
                uint8_t bmMiddleButton : 1;
                uint8_t bmDummy : 5;
        };
        uint16_t dX;
        uint16_t dY;
};

// Override HIDComposite to be able to select which interface we want to hook into
class FlexibleUsbHidDevice : public HIDComposite
{
public:
    FlexibleUsbHidDevice(USB *p) : HIDComposite(p) {};

protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);

    void MyParseMouseData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    void MyParseKeyboardData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
};


bool FlexibleUsbHidDevice::SelectInterface(uint8_t iface, uint8_t proto)
{
    return true;
}


// Will be called for all HID data received from the USB interface
void FlexibleUsbHidDevice::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    char mystring[256];
    uint8_t protocol = this->hidInterfaces[ep-1].bmProtocol;

    Serial.print(" Protocol is:");
    if(protocol == USB_HID_PROTOCOL_KEYBOARD)
    {
        Serial.print("USB_HID_PROTOCOL_KEYBOARD");
    }
    else if(protocol == USB_HID_PROTOCOL_MOUSE){
        Serial.print("USB_HID_PROTOCOL_MOUSE");
    }
    else if(protocol == USB_HID_PROTOCOL_NONE){
        Serial.print("USB_HID_PROTOCOL_NONE");
    }
    else{
        Serial.print("!!! UNKNOWN PROTOCOL !!!");
        Serial.print(this->hidInterfaces[ep].bmProtocol);
    }



    if(protocol == USB_HID_PROTOCOL_KEYBOARD){
        if(len >= sizeof(KBDINFO)){

            KBDINFO *ki = (KBDINFO*)buf;
            sprintf(mystring,"Keyboard: Key1:%02X Key2:%02X Key3:%02X Key4:%02X Key5:%02X\n\r",
                ki->Keys[0], ki->Keys[1], ki->Keys[2], ki->Keys[3], ki->Keys[4]);
            Serial.print(mystring);
        }
        // MyParseKeyboardData(hid,ep,is_rpt_id,len,buf);
    }
    if(protocol == USB_HID_PROTOCOL_MOUSE){
        // MyParseMouseData(hid,ep,is_rpt_id,len,buf);
    }

    // // char mystring[256];
    // // uint8_t data_buff[1024];
    // if (len && buf)  {

    //     Serial.print(F("\r\n"));
    //     // Serial.print("epAttribs:");
    //     // for(int i=0; i<sizeof(EpInfo); i++)
    //     // {
    //     //     Serial.print(" ");
    //     //     Serial.print((byte)((byte*)this->epInfo[ep].epAttribs)[i],HEX);
    //     // }

    //     Serial.print(" Address:");
    //     Serial.print(this->GetAddress());

    //     Serial.print(" PID:");
    //     Serial.print(this->PID,HEX);
    //     Serial.print(" VID:");
    //     Serial.print(this->VID,HEX);

    //     Serial.print(" EP:");

    //     Serial.print(ep);
    //     Serial.print(" bConfNum:");
    //     Serial.print(this->bConfNum);
    //     Serial.print(" is_rpt_id:");
    //     Serial.print(is_rpt_id);
    //     Serial.print(" sizeof(MOUSEINFO):");
    //     Serial.print(sizeof(MOUSEINFO));
    //     Serial.print(" sizeof(KBDINFO):");
    //     Serial.print(sizeof(KBDINFO));
    //     Serial.print(" sizeof(ABSOLUTEMOUSEINFO):");
    //     Serial.print(sizeof(ABSOLUTEMOUSEINFO));

    //     Serial.print(" len:");
    //     Serial.print(len);
        
    //     Serial.print(" -- ");
    //     for (uint8_t i = 0; i < len; i++) {
    //         if (buf[i] < 16) Serial.print(F("0"));
    //         Serial.print(buf[i], HEX);
    //         Serial.print(F(" "));
    //     }

    //     Serial.println("");

        // for(int i=0; i<maxHidInterfaces; i++){
        //     Serial.print(" bmInterface:");
        //     Serial.print( this->hidInterfaces[i].bmInterface,HEX);
        //     Serial.print(" bmAltSet:");
        //     Serial.print( this->hidInterfaces[i].bmAltSet,HEX);
        //     Serial.print(" bmProtocol:");
        //     Serial.println( this->hidInterfaces[i].bmProtocol,HEX);
        // }
    // }



    // if(ep == USB_HID_PROTOCOL_KEYBOARD){
    //     if(len >= sizeof(KBDINFO)){

    //         KBDINFO *ki = (KBDINFO*)buf;
    //         sprintf(mystring,"Keyboard: Key1:%02X Key2:%02X Key3:%02X Key4:%02X Key5:%02X\n\r",
    //             ki->Keys[0], ki->Keys[1], ki->Keys[2], ki->Keys[3], ki->Keys[4]);
    //         Serial.print(mystring);
    //     }
    //     else{
    //         Serial.print("!!!! Unknown keyboard messsage!!");
    //     }
    // }

    // if(ep == USB_HID_PROTOCOL_MOUSE){
    //     if(len >= sizeof(ABSOLUTEMOUSEINFO)){
    //         ABSOLUTEMOUSEINFO *mi = (ABSOLUTEMOUSEINFO*)buf;
    //         sprintf(mystring,"Absolute Mouse - Left:%02X Right:%02X Mid:%02X X:%d Y:%d\n\r",
    //             mi->bmLeftButton, mi->bmRightButton, mi->bmMiddleButton, mi->dX, mi->dY);
    //         Serial.print(mystring);
    //     }
    //     else if(len >= sizeof(MOUSEINFO)){

    //         MOUSEINFO *mi = (MOUSEINFO*)buf;
    //         sprintf(mystring,"Mouse - Left:%02X Right:%02X Mid:%02X DX:%d DY:%d\n\r",
    //             mi->bmLeftButton, mi->bmRightButton, mi->bmMiddleButton, mi->dX, mi->dY);
    //         Serial.print(mystring);
    //     }
    // }





}


USB         Usb;
USBHub      Hub(&Usb);
USBHub      Hub1(&Usb);
USBHub      Hub2(&Usb);
USBHub      Hub3(&Usb);
FlexibleUsbHidDevice hidSelector(&Usb);
FlexibleUsbHidDevice hidSelector2(&Usb);
FlexibleUsbHidDevice hidSelector3(&Usb);
FlexibleUsbHidDevice hidSelector4(&Usb);
FlexibleUsbHidDevice hidSelector5(&Usb);

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