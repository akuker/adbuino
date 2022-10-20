// Functions for printing out USB device information
// Based upon the USB_Desc Arduino example sketch, which is
// part of the USB Host Shield 2.0 library:
//
// Revision 2.0 of MAX3421E-based USB Host Shield Library.
// Supports HID devices, FTDI, ADK, ACM, PL2303, Bluetooth HID 
// devices, SPP communication and mass storage devices. 
// Furthermore it supports PS3, PS4, PS5, PS Buzz, Wii, Switch 
// Pro and Xbox controllers.
//
// Author: Oleg Mazurov (Circuits@Home)
//
// Maintainer: Oleg Mazurov (Circuits@Home)  

#include <stdint.h>
#include <hidboot.h>
#include <usbhub.h>

extern USB Usb;

void printProgStr(const char* str);
void printconfdescr( uint8_t* descr_ptr );
void printintfdescr( uint8_t* descr_ptr );
void print_hex(int v, int num_places);
void printconfdescr( uint8_t* descr_ptr );
void printhubdescr(uint8_t *descrptr, uint8_t addr);
uint8_t getconfdescr( uint8_t addr, uint8_t conf );
void printconfdescr( uint8_t* descr_ptr );
void printintfdescr( uint8_t* descr_ptr );
void printepdescr( uint8_t* descr_ptr );
void printunkdescr( uint8_t* descr_ptr );

void printProgStr(const char* str);

void PrintAllDescriptors(UsbDevice *pdev);
void PrintAllAddresses(UsbDevice *pdev);

