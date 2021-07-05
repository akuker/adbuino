// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <usbhub.h>
#include "pgmstrings.h"


USB     Usb;
USBHub  Hub1(&Usb);
USBHub  Hub2(&Usb);
USBHub  Hub3(&Usb);
USBHub  Hub4(&Usb);

uint32_t next_time;


class MouseRptParser : public MouseReportParser
{
protected:
	void OnMouseMove	(MOUSEINFO *mi);
	void OnLeftButtonUp	(MOUSEINFO *mi);
	void OnLeftButtonDown	(MOUSEINFO *mi);
	void OnRightButtonUp	(MOUSEINFO *mi);
	void OnRightButtonDown	(MOUSEINFO *mi);
	void OnMiddleButtonUp	(MOUSEINFO *mi);
	void OnMiddleButtonDown	(MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    Serial.print("dx=");
    Serial.print(mi->dX, DEC);
    Serial.print(" dy=");
    Serial.println(mi->dY, DEC);
};
void MouseRptParser::OnLeftButtonUp	(MOUSEINFO *mi)
{
    Serial.println("L Butt Up");
};
void MouseRptParser::OnLeftButtonDown	(MOUSEINFO *mi)
{
    Serial.println("L Butt Dn");
};
void MouseRptParser::OnRightButtonUp	(MOUSEINFO *mi)
{
    Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi)
{
    Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp	(MOUSEINFO *mi)
{
    Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown	(MOUSEINFO *mi)
{
    Serial.println("M Butt Dn");
};


void PrintAllAddresses(UsbDevice *pdev)
{
  UsbDeviceAddress adr;
  adr.devAddress = pdev->address.devAddress;
  Serial.print("Addr:");
  Serial.print(adr.devAddress, HEX);
  Serial.print("(");
  Serial.print(adr.bmHub, HEX);
  Serial.print(".");
  Serial.print(adr.bmParent, HEX);
  Serial.print(".");
  Serial.print(adr.bmAddress, HEX);
  Serial.println(")");
}

void PrintAddress(uint8_t addr)
{
  UsbDeviceAddress adr;
  adr.devAddress = addr;
  Serial.print("\r\nADDR:\t");
  Serial.println(adr.devAddress, HEX);
  Serial.print("DEV:\t");
  Serial.println(adr.bmAddress, HEX);
  Serial.print("PRNT:\t");
  Serial.println(adr.bmParent, HEX);
  Serial.print("HUB:\t");
  Serial.println(adr.bmHub, HEX);
}

void PrintDescriptors(uint8_t addr)
{
  uint8_t rcode = 0;
  uint8_t num_conf = 0;

  rcode = getdevdescr( (uint8_t)addr, num_conf );
  if ( rcode ) {
    printProgStr(Gen_Error_str);
    print_hex( rcode, 8 );
  }
  Serial.print("\r\n");

  for (uint8_t i = 0; i < num_conf; i++) {
    rcode = getconfdescr( addr, i );                 // get configuration descriptor
    if ( rcode ) {
      printProgStr(Gen_Error_str);
      print_hex(rcode, 8);
    }
    Serial.println("\r\n");
  }
}

void PrintAllDescriptors(UsbDevice *pdev)
{
  Serial.println("\r\n");
  print_hex(pdev->address.devAddress, 8);
  Serial.println("\r\n--");
  getallstrdescr(pdev->address.devAddress);
  PrintDescriptors( pdev->address.devAddress );
}


uint8_t getdevdescr( uint8_t addr, uint8_t &num_conf )
{
  USB_DEVICE_DESCRIPTOR buf;
  uint8_t rcode;
  rcode = Usb.getDevDescr( addr, 0, DEV_DESCR_LEN, ( uint8_t *)&buf );
  if ( rcode ) {
    return ( rcode );
  }
  printProgStr(Dev_Header_str);
  printProgStr(Dev_Length_str);
  print_hex( buf.bLength, 8 );
  printProgStr(Dev_Type_str);
  print_hex( buf.bDescriptorType, 8 );
  printProgStr(Dev_Version_str);
  print_hex( buf.bcdUSB, 16 );
  printProgStr(Dev_Class_str);
  print_hex( buf.bDeviceClass, 8 );
  printProgStr(Dev_Subclass_str);
  print_hex( buf.bDeviceSubClass, 8 );
  printProgStr(Dev_Protocol_str);
  print_hex( buf.bDeviceProtocol, 8 );
  printProgStr(Dev_Pktsize_str);
  print_hex( buf.bMaxPacketSize0, 8 );
  printProgStr(Dev_Vendor_str);
  print_hex( buf.idVendor, 16 );
  printProgStr(Dev_Product_str);
  print_hex( buf.idProduct, 16 );
  printProgStr(Dev_Revision_str);
  print_hex( buf.bcdDevice, 16 );
  printProgStr(Dev_Mfg_str);
  print_hex( buf.iManufacturer, 8 );
  printProgStr(Dev_Prod_str);
  print_hex( buf.iProduct, 8 );
  printProgStr(Dev_Serial_str);
  print_hex( buf.iSerialNumber, 8 );
  printProgStr(Dev_Nconf_str);
  print_hex( buf.bNumConfigurations, 8 );
  num_conf = buf.bNumConfigurations;
  return ( 0 );
}

void printhubdescr(uint8_t *descrptr, uint8_t addr)
{
  HubDescriptor  *pHub = (HubDescriptor*) descrptr;
  uint8_t        len = *((uint8_t*)descrptr);

  printProgStr(PSTR("\r\n\r\nHub Descriptor:\r\n"));
  printProgStr(PSTR("bDescLength:\t\t"));
  Serial.println(pHub->bDescLength, HEX);

  printProgStr(PSTR("bDescriptorType:\t"));
  Serial.println(pHub->bDescriptorType, HEX);

  printProgStr(PSTR("bNbrPorts:\t\t"));
  Serial.println(pHub->bNbrPorts, HEX);

  printProgStr(PSTR("LogPwrSwitchMode:\t"));
  Serial.println(pHub->LogPwrSwitchMode, BIN);

  printProgStr(PSTR("CompoundDevice:\t\t"));
  Serial.println(pHub->CompoundDevice, BIN);

  printProgStr(PSTR("OverCurrentProtectMode:\t"));
  Serial.println(pHub->OverCurrentProtectMode, BIN);

  printProgStr(PSTR("TTThinkTime:\t\t"));
  Serial.println(pHub->TTThinkTime, BIN);

  printProgStr(PSTR("PortIndicatorsSupported:"));
  Serial.println(pHub->PortIndicatorsSupported, BIN);

  printProgStr(PSTR("Reserved:\t\t"));
  Serial.println(pHub->Reserved, HEX);

  printProgStr(PSTR("bPwrOn2PwrGood:\t\t"));
  Serial.println(pHub->bPwrOn2PwrGood, HEX);

  printProgStr(PSTR("bHubContrCurrent:\t"));
  Serial.println(pHub->bHubContrCurrent, HEX);

  for (uint8_t i = 7; i < len; i++)
    print_hex(descrptr[i], 8);

  //for (uint8_t i=1; i<=pHub->bNbrPorts; i++)
  //    PrintHubPortStatus(&Usb, addr, i, 1);
}

uint8_t getconfdescr( uint8_t addr, uint8_t conf )
{
  uint8_t buf[ BUFSIZE ];
  uint8_t* buf_ptr = buf;
  uint8_t rcode;
  uint8_t descr_length;
  uint8_t descr_type;
  uint16_t total_length;
  rcode = Usb.getConfDescr( addr, 0, 4, conf, buf );  //get total length
  LOBYTE( total_length ) = buf[ 2 ];
  HIBYTE( total_length ) = buf[ 3 ];
  if ( total_length > 256 ) {   //check if total length is larger than buffer
    printProgStr(Conf_Trunc_str);
    total_length = 256;
  }
  rcode = Usb.getConfDescr( addr, 0, total_length, conf, buf ); //get the whole descriptor
  while ( buf_ptr < buf + total_length ) { //parsing descriptors
    descr_length = *( buf_ptr );
    descr_type = *( buf_ptr + 1 );
    switch ( descr_type ) {
      case ( USB_DESCRIPTOR_CONFIGURATION ):
        printconfdescr( buf_ptr );
        break;
      case ( USB_DESCRIPTOR_INTERFACE ):
        printintfdescr( buf_ptr );
        break;
      case ( USB_DESCRIPTOR_ENDPOINT ):
        printepdescr( buf_ptr );
        break;
      case 0x29:
        printhubdescr( buf_ptr, addr );
        break;
      default:
        printunkdescr( buf_ptr );
        break;
    }//switch( descr_type
    buf_ptr = ( buf_ptr + descr_length );    //advance buffer pointer
  }//while( buf_ptr <=...
  return ( rcode );
}

// function to get all string descriptors
uint8_t getallstrdescr(uint8_t addr)
{
  uint8_t rcode = 0;
  Usb.Task();
  if ( Usb.getUsbTaskState() >= USB_STATE_CONFIGURING ) { // state configuring or higher
    USB_DEVICE_DESCRIPTOR buf;
    rcode = Usb.getDevDescr( addr, 0, DEV_DESCR_LEN, ( uint8_t *)&buf );
    if ( rcode ) {
      return ( rcode );
    }
    Serial.println("String Descriptors:");
    if ( buf.iManufacturer > 0 ) {
      Serial.print("Manufacturer:\t\t");
      rcode = getstrdescr( addr, buf.iManufacturer );   // get manufacturer string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
    if ( buf.iProduct > 0 ) {
      Serial.print("Product:\t\t");
      rcode = getstrdescr( addr, buf.iProduct );        // get product string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
    if ( buf.iSerialNumber > 0 ) {
      Serial.print("Serial:\t\t\t");
      rcode = getstrdescr( addr, buf.iSerialNumber );   // get serial string
      if ( rcode ) {
        Serial.println( rcode, HEX );
      }
      Serial.print("\r\n");
    }
  }
  return rcode;
}

//  function to get single string description
uint8_t getstrdescr( uint8_t addr, uint8_t idx )
{
  uint8_t buf[ 256 ];
  uint8_t rcode;
  uint8_t length;
  uint8_t i;
  uint16_t langid;
  rcode = Usb.getStrDescr( addr, 0, 1, 0, 0, buf );  //get language table length
  if ( rcode ) {
    Serial.println("Error retrieving LangID table length");
    return ( rcode );
  }
  length = buf[ 0 ];      //length is the first byte
  rcode = Usb.getStrDescr( addr, 0, length, 0, 0, buf );  //get language table
  if ( rcode ) {
    Serial.print("Error retrieving LangID table ");
    return ( rcode );
  }
  langid = (buf[3] << 8) | buf[2];
  rcode = Usb.getStrDescr( addr, 0, 1, idx, langid, buf );
  if ( rcode ) {
    Serial.print("Error retrieving string length ");
    return ( rcode );
  }
  length = buf[ 0 ];
  rcode = Usb.getStrDescr( addr, 0, length, idx, langid, buf );
  if ( rcode ) {
    Serial.print("Error retrieving string ");
    return ( rcode );
  }
  for ( i = 2; i < length; i += 2 ) {   //string is UTF-16LE encoded
    Serial.print((char) buf[i]);
  }
  return ( rcode );
}

/* prints hex numbers with leading zeroes */
// copyright, Peter H Anderson, Baltimore, MD, Nov, '07
// source: http://www.phanderson.com/arduino/arduino_display.html
void print_hex(int v, int num_places)
{
  int mask = 0, n, num_nibbles, digit;

  for (n = 1; n <= num_places; n++) {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask; // truncate v to specified number of places

  num_nibbles = num_places / 4;
  if ((num_places % 4) != 0) {
    ++num_nibbles;
  }
  do {
    digit = ((v >> (num_nibbles - 1) * 4)) & 0x0f;
    Serial.print(digit, HEX);
  }
  while (--num_nibbles);
}

/* function to print configuration descriptor */
void printconfdescr( uint8_t* descr_ptr )
{
  USB_CONFIGURATION_DESCRIPTOR* conf_ptr = ( USB_CONFIGURATION_DESCRIPTOR* )descr_ptr;
  printProgStr(Conf_Header_str);
  printProgStr(Conf_Totlen_str);
  print_hex( conf_ptr->wTotalLength, 16 );
  printProgStr(Conf_Nint_str);
  print_hex( conf_ptr->bNumInterfaces, 8 );
  printProgStr(Conf_Value_str);
  print_hex( conf_ptr->bConfigurationValue, 8 );
  printProgStr(Conf_String_str);
  print_hex( conf_ptr->iConfiguration, 8 );
  printProgStr(Conf_Attr_str);
  print_hex( conf_ptr->bmAttributes, 8 );
  printProgStr(Conf_Pwr_str);
  print_hex( conf_ptr->bMaxPower, 8 );
  return;
}

/* function to print interface descriptor */
void printintfdescr( uint8_t* descr_ptr )
{
  USB_INTERFACE_DESCRIPTOR* intf_ptr = ( USB_INTERFACE_DESCRIPTOR* )descr_ptr;
  printProgStr(Int_Header_str);
  printProgStr(Int_Number_str);
  print_hex( intf_ptr->bInterfaceNumber, 8 );
  printProgStr(Int_Alt_str);
  print_hex( intf_ptr->bAlternateSetting, 8 );
  printProgStr(Int_Endpoints_str);
  print_hex( intf_ptr->bNumEndpoints, 8 );
  printProgStr(Int_Class_str);
  print_hex( intf_ptr->bInterfaceClass, 8 );
  printProgStr(Int_Subclass_str);
  print_hex( intf_ptr->bInterfaceSubClass, 8 );
  printProgStr(Int_Protocol_str);
  print_hex( intf_ptr->bInterfaceProtocol, 8 );
  printProgStr(Int_String_str);
  print_hex( intf_ptr->iInterface, 8 );
  return;
}

/* function to print endpoint descriptor */
void printepdescr( uint8_t* descr_ptr )
{
  USB_ENDPOINT_DESCRIPTOR* ep_ptr = ( USB_ENDPOINT_DESCRIPTOR* )descr_ptr;
  printProgStr(End_Header_str);
  printProgStr(End_Address_str);
  print_hex( ep_ptr->bEndpointAddress, 8 );
  printProgStr(End_Attr_str);
  print_hex( ep_ptr->bmAttributes, 8 );
  printProgStr(End_Pktsize_str);
  print_hex( ep_ptr->wMaxPacketSize, 16 );
  printProgStr(End_Interval_str);
  print_hex( ep_ptr->bInterval, 8 );

  return;
}

/*function to print unknown descriptor */
void printunkdescr( uint8_t* descr_ptr )
{
  uint8_t length = *descr_ptr;
  uint8_t i;
  printProgStr(Unk_Header_str);
  printProgStr(Unk_Length_str);
  print_hex( *descr_ptr, 8 );
  printProgStr(Unk_Type_str);
  print_hex( *(descr_ptr + 1 ), 8 );
  printProgStr(Unk_Contents_str);
  descr_ptr += 2;
  for ( i = 0; i < length; i++ ) {
    print_hex( *descr_ptr, 8 );
    descr_ptr++;
  }
}

/* Print a string from Program Memory directly to save RAM */
void printProgStr(const char* str)
{
  char c;
  if (!str) return;
  while ((c = pgm_read_byte(str++)))
    Serial.print(c);
}








class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
};

//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser                               MousePrs;
KbdRptParser Prs;

void setup()
{


  
  Serial.begin( 9600 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);

  HidMouse.SetReportParser(0, &MousePrs);

  next_time = (uint32_t)millis() + 10000;

}

int first_time=1;
void loop()
{
  Usb.Task();

  if(first_time){
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {
    if ((int32_t)((uint32_t)millis() - next_time) >= 0L) {
      Usb.ForEachUsbDevice(&PrintAllDescriptors);
      Usb.ForEachUsbDevice(&PrintAllAddresses);
      first_time=0;

      // while ( 1 );                          //stop
    }
  }
}

}