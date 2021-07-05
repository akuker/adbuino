#include "adb.h"
#include <stdio.h>


extern uint8_t mousepending;
extern uint8_t kbdpending  ;
extern uint8_t kbdskip     ;
extern uint16_t kbdprev0   ;
extern uint16_t mousereg0  ;
extern uint16_t kbdreg0    ;
extern uint8_t kbdsrq      ;
extern uint8_t mousesrq    ;
extern uint8_t modifierkeys;
extern uint32_t kbskiptimer;

#ifdef PICO_SDK
uint32_t millis(){
    return to_ms_since_boot(get_absolute_time());
}
#endif

void handle_adb_cmd(uint8_t cmd){

    // see if it is addressed to us
  if(((cmd >> 4) & 0x0F) == 3) {
    switch(cmd & 0x0F) {
      case 0xC: // talk register 0
        if(mousepending) {
          _delay_us(180); // stop to start time / interframe delay
        //   ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
            adb_pin_out();
          place_bit1(); // start bit
          send_byte((mousereg0 >> 8) & 0x00FF);
          send_byte(mousereg0 & 0x00FF);
          place_bit0(); // stop bit
        //   ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
            adb_pin_in();
          mousepending = 0;
          mousesrq = 0;
        }
        break;
      default:
        printf("Unknown cmd: %02X",cmd);
        break;
    }
  } else {
    if(mousepending) mousesrq = 1;
  }
  
  if(((cmd >> 4) & 0x0F) == 2) {
    uint8_t adbleds;
    switch(cmd & 0x0F) {
      case 0xC: // talk register 0
        if(kbdpending) {
          
          if (kbdskip) {
            kbdskip = 0;
            // Serial.println("Skipping invalid 255 signal and sending keyup instead");

            // Send a 'key released' code to avoid ADB sticking to the previous key
            kbdprev0 |= 0x80;
            kbdreg0 = (kbdprev0 << 8) | 0xFF;

            // Save timestamp 
            kbskiptimer = millis();
          } else if (millis() - kbskiptimer < 90) {
          // Check timestamp and don't process the key event if it came right after a 255
          // This is meant to avoid a glitch where releasing a key sends 255->keydown instead
            printf("Too little time since bugged keyup, skipping this keydown event");
            kbdpending = 0;
            break;
          } 
          
          // Serial.print("Sending keycode ");
          // Serial.print(kbdreg0);
          // Serial.println(" to ADB");
          
          kbdsrq = 0;
          _delay_us(180); // stop to start time / interframe delay
        //   ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
        adb_pin_out();
          place_bit1(); // start bit
          send_byte((kbdreg0 >> 8) & 0x00FF);
          send_byte(kbdreg0 & 0x00FF);
          place_bit0(); // stop bit
        //   ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
        adb_pin_in();
          kbdpending = 0;
        }
        break;
      case 0xD: // talk register 1
        break;
      case 0xE: // talk register 2
        adbleds = 0xFF;
        // if(!(ps2ledstate & kPS2LEDCaps)) adbleds &= ~2;
        // if(!(ps2ledstate & kPS2LEDScroll)) adbleds &= ~4;
        // if(!(ps2ledstate & kPS2LEDNum)) adbleds &= ~1;
        
        _delay_us(180); // stop to start time / interframe delay
        // ADB_DDR |= 1<<ADB_DATA_BIT;  // set output
        adb_pin_out();
        place_bit1(); // start bit
        send_byte(modifierkeys);
        send_byte(adbleds);
        place_bit0(); // stop bit
        // ADB_DDR &= ~(1<<ADB_DATA_BIT); // set input
        adb_pin_in();
        
        break;
      case 0xF: // talk register 3
        // sets device address
        break;
      default:
        printf("Unknown cmd: %02X\n", cmd);
        break;
    }
  } else {
    if(kbdpending) kbdsrq = 1;
  }
}
