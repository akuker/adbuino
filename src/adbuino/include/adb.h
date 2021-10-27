#pragma once

#include <stdint.h>
#include "Arduino.h"


class AdbInterface {
  public:
    void Init();
    //void ReadAdbCommand();
    uint8_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(uint8_t cmd);

    protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();

  private:
    const uint8_t m_adb_gpio = 3;
    const uint8_t m_pwr_gpio = 2;

    enum e_adb_device_address : uint8_t {
      e_adb_macintosh_host = 0x0,
      e_adb_protection_device = 0x1,
      e_adb_addr_keyboard = 0x2,
      e_adb_addr_mouse = 0x3,
      e_adb_addr_tablet = 0x4,
      e_adb_addr_modem = 0x5,
      e_adb_addr_reserved = 0x6,
      e_adb_adr_appliances = 0x7,
    };

    // These need to be looked upon the arduino schematic to figure out
    // which bank and databit the ADB pin is connected to
    #define ADB_PORT      PORTD
    #define ADB_PIN       PIND
    #define ADB_DDR       DDRD
    #define ADB_DATA_BIT  3


    void data_lo();
    void data_hi();
    uint8_t data_in();
    void adb_pin_out();
    void adb_pin_in();

    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    void place_bit0(void);
    void place_bit1(void);
    void send_byte(uint8_t data);
};
