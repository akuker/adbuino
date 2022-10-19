#pragma once

#include "Arduino.h"
#include <stdint.h>

// _delay_us's parameter must be a compile time integer, and thus cannot be a inline class method
#define adb_delay_us(delay) (_delay_us(delay))

class AdbInterfacePlatform {
  public:
    void Init();

  protected:
    // These need to be looked upon the arduino schematic to figure out
    // which bank and databit the ADB pin is connected to
    #define ADB_PORT      PORTD
    #define ADB_PIN       PIND
    #define ADB_DDR       DDRD
    #define ADB_DATA_BIT  3
    
    void data_lo();
    void data_hi();
    void adb_pin_out();
    void adb_pin_in();
    uint8_t data_in();

    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);

};


inline void AdbInterfacePlatform::data_lo()
{
  (ADB_DDR |= (1 << ADB_DATA_BIT));
  (ADB_PORT &= ~(1 << ADB_DATA_BIT));
}

inline void AdbInterfacePlatform::data_hi()
{
  (ADB_DDR &= ~(1 << ADB_DATA_BIT));
}
inline uint8_t AdbInterfacePlatform::data_in()
{
  return (ADB_PIN & (1 << ADB_DATA_BIT));
}

inline void AdbInterfacePlatform::adb_pin_out()
{
  (ADB_DDR |= (1 << ADB_DATA_BIT));
}
inline void AdbInterfacePlatform::adb_pin_in()
{
  (ADB_DDR &= ~(1 << ADB_DATA_BIT));
}

inline uint16_t AdbInterfacePlatform::wait_data_lo(uint32_t us)
{
  do
  {
    if (!data_in())
      break;
    _delay_us(1 - (6 * 1000000.0 / F_CPU));
  } while (--us);
  return us;
}

inline uint16_t AdbInterfacePlatform::wait_data_hi(uint32_t us)
{
  do
  {
    if (data_in())
      break;
    _delay_us(1 - (6 * 1000000.0 / F_CPU));
  } while (--us);
  return us;
}


