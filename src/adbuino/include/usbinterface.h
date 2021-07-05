#include "Arduino.h"

// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


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

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

class UsbInterface{
    public:

    void Init();
    void RunTask();
void PrintAllAddresses(UsbDevice *pdev);
void PrintAddress(uint8_t addr);
void PrintDescriptors(uint8_t addr);


void PrintAllDescriptors(UsbDevice *pdev);

void printhubdescr(uint8_t *descrptr, uint8_t addr);
uint8_t getconfdescr( uint8_t addr, uint8_t conf );
// function to get all string descriptors
uint8_t getallstrdescr(uint8_t addr);
//  function to get single string description
uint8_t getstrdescr( uint8_t addr, uint8_t idx );


/* prints hex numbers with leading zeroes */
// copyright, Peter H Anderson, Baltimore, MD, Nov, '07
// source: http://www.phanderson.com/arduino/arduino_display.html
void print_hex(int v, int num_places);

/* function to print configuration descriptor */
void printconfdescr( uint8_t* descr_ptr );


/* function to print interface descriptor */
void printintfdescr( uint8_t* descr_ptr );

/* function to print endpoint descriptor */
void printepdescr( uint8_t* descr_ptr );

/*function to print unknown descriptor */
void printunkdescr( uint8_t* descr_ptr );

/* Print a string from Program Memory directly to save RAM */
void printProgStr(const char* str);


uint8_t getdevdescr( uint8_t addr, uint8_t &num_conf );
    private: 
        USB     *Usb;
        USBHub  *Hub1;
        USBHub  *Hub2;
        USBHub  *Hub3;
        USBHub  *Hub4;
        HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    *HidKeyboard;
        HIDBoot<USB_HID_PROTOCOL_MOUSE>    *HidMouse;

        MouseRptParser *MousePrs;
        KbdRptParser *KeyboardPrs;
        uint32_t next_time;

};