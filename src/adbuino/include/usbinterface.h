#include "Arduino.h"

// Example code from: https://platformio.org/lib/show/59/USB-Host-Shield-20
#include <hidboot.h>
#include <usbhub.h>
#include <ArduinoQueue.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

extern uint8_t usb_keycode_to_adb_code(uint8_t usb_code);

class MouseRptParser : public MouseReportParser
{
public:
    bool MouseChanged();
    int32_t GetDeltaX();
    int32_t GetDeltaY();
    void ResetMouseMovement();
    uint16_t GetAdbRegister0();
    bool MouseButtonIsPressed();

protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    uint32_t EightBitToSevenBitSigned(int8_t value);


private:
    int8_t m_movedx = 0;
    int8_t m_movedy = 0;
    bool m_mouse_button_is_pressed = false;
    bool m_mouse_button_changed = false;
};

class KeyEvent
{
public:
    static const uint8_t NoKey = 0xFF;
    static const uint8_t KeyDown = 0x01;
    static const uint8_t KeyUp = 0x02;
    inline uint8_t GetKeycode() { return m_keycode; }
    inline bool IsKeyUp() { return m_key_updown == KeyUp; }
    inline bool IsKeyDown() { return m_key_updown == KeyDown; }
    KeyEvent(uint8_t KeyCode, uint8_t KeyUpDown)
    {
        m_key_updown = KeyUpDown;
        m_keycode = KeyCode;
    }

protected:
    uint8_t m_keycode;
    uint8_t m_key_updown;
};

class KbdRptParser : public KeyboardReportParser
{
public:
    KbdRptParser();
    void PrintKey(uint8_t mod, uint8_t key);

    uint16_t GetAdbRegister0();
    uint16_t GetAdbRegister2();

    KeyEvent GetKeyEvent();
    bool PendingKeyboardEvent();

protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown(uint8_t mod, uint8_t key);
    void OnKeyUp(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
    
    uint8_t m_last_key_pressed;
    uint8_t m_last_key_up_or_down;

    uint8_t m_previous_key_pressed;
    MODIFIERKEYS m_modifier_keys;

    // Flag to indicated that the key was released
    const uint16_t KeyReleasedFlag = 0x80;

    // Flags for special modifier keys that are used in the ADB
    // protocol, but not handled automatically by the Arduino
    // USB library
    uint16_t m_custom_mod_keys;
    // Flags for the custom ADB-specific modifier keys
    static const int DeleteFlag = 0;
    static const int CapsLockFlag = 1;
    static const int ResetFlag = 2;
    static const int NumLockFlag = 3;
    static const int ScrollLockFlag = 4;
    static const int Led3ScrollLockFlag = 5;
    static const int Led2CapsLockFlag = 6;
    static const int Led1NumLockFlag = 7;

    ArduinoQueue<KeyEvent*> *m_keyboard_events;

};





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

    MouseRptParser *MousePrs;
    KbdRptParser *KeyboardPrs;
    uint32_t next_time;
};