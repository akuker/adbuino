#include "flexible_hid.h"
#include "hidboot.h"

bool FlexibleUsbHidDevice::SelectInterface(uint8_t iface, uint8_t proto)
{
    return true;
}


// Will be called for all HID data received from the USB interface
void FlexibleUsbHidDevice::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    char mystring[256];
    uint8_t protocol = this->hidInterfaces[ep-1].bmProtocol;

    Serial.print(" EP:");
    Serial.print(ep);

    Serial.print(" [");
    Serial.print(len);
    Serial.print("] ");
    for(int i=0; i<len; i++){
        Serial.print(buf[i],HEX);
        Serial.print(" ");
    }


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
        Serial.println("");
    }

    if(protocol == USB_HID_PROTOCOL_NONE){
        Serial.println("");
        // Serial.print("devsubclassok:");
        // Serial.print(hid->DEVSUBCLASSOK(),HEX);

        // Serial.print(" Address:");
        // Serial.print(this->GetAddress());

        // Serial.print(" PID:");
        // Serial.print(this->PID,HEX);
        // Serial.print(" VID:");
        // Serial.print(this->VID,HEX);

        
        // Serial.print(" bConfNum:");
        // Serial.print(this->bConfNum);
        // Serial.print(" is_rpt_id:");
        // Serial.print(is_rpt_id);
        // Serial.print(" sizeof(MOUSEINFO):");
        // Serial.print(sizeof(MOUSEINFO));
        // Serial.print(" sizeof(KBDINFO):");
        // Serial.print(sizeof(KBDINFO));
        // Serial.print(" sizeof(ABSOLUTEMOUSEINFO):");
        // Serial.print(sizeof(ABSOLUTEMOUSEINFO));

        
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