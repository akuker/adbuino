/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 * QuokkADB is derived from adbuino:  https://github.com/akuker/adbuino
 * 
 * Original code derived from adbuino:  https://github.com/akuker/adbuino
 * 26 September 2022 - Modifed by Rabbit Hole Computing LLC
 */

#pragma once

#include <stdint.h>

class AdbInterface {
  public:
    void Init();
    uint8_t ReceiveCommand(uint8_t srq);
    void ProcessCommand(uint8_t cmd);

    protected:
    uint16_t GetAdbRegister3Keyboard();
    uint16_t GetAdbRegister3Mouse();

  private:
    void data_lo();
    void data_hi();
    uint8_t data_in();


    uint16_t wait_data_lo(uint32_t us);
    uint16_t wait_data_hi(uint32_t us);
    void place_bit0(void);
    void place_bit1(void);
    void send_byte(uint8_t data);
};
