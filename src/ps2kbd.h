#define kPS2ErrorCode 0xFF00
#define kPS2RetryCode 0xFE00
#define kPS2LEDScroll 1
#define kPS2LEDNum    2
#define kPS2LEDCaps   4
#define  kPS2KbdClockPin   2
#define  kPS2KbdDataPin    1
#define  kPS2KbdModePort   DDRB
#define  kPS2KbdOutPort    PORTB
#define  kPS2KbdInPort     PINB
#define  kPS2MouseClockPin (1<<7)
#define  kPS2MouseDataPin  (1<<6)
#define  kPS2MouseModePort DDRD
#define  kPS2MouseOutPort  PORTD
#define  kPS2MouseInPort   PIND
#define  kPS2MouseClockArduinoPin 7
#define  kPS2MouseDataArduinoPin  6
uint16_t PollPS2KbdFrame(uint8_t clock, uint8_t data, uint32_t polltimeout);
void SendPS2KbdCommand(uint8_t cmd, uint8_t clock, uint8_t data);
void SetPS2Repeat(void);
void SetPS2LEDState(uint8_t cmd);
uint8_t HandlePS2Keyboard(uint8_t *code, uint8_t *keyup, uint8_t *extended);
void InitPS2Keyboard(void);
uint16_t PollPS2MouseFrame(uint8_t clock, uint8_t data, uint32_t polltimeout);
void SendPS2MouseCommand(uint8_t cmd, uint8_t clock, uint8_t data);
#include <Arduino.h>
uint32_t ps2ledstate = 0;
uint16_t PollPS2KbdFrame(uint8_t clock, uint8_t data, uint32_t polltimeout);
static inline uint16_t wait_lo(volatile uint8_t *port, uint8_t pin, uint16_t us)
{
    do {
        if ( !(*port & pin) )
            break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
    return us;
}
static inline uint16_t wait_hi(volatile uint8_t *port, uint8_t pin, uint16_t us)
{
    do {
        if ( *port & pin )
            break;
        _delay_us(1 - (6 * 1000000.0 / F_CPU));
    }
    while ( --us );
    return us;
}
void InitPS2Keyboard(void)
{
  //pinMode(8, INPUT);
  //pinMode(9, INPUT);
  kPS2KbdModePort &= ~(kPS2KbdClockPin | kPS2KbdDataPin);
  kPS2KbdOutPort |= kPS2KbdClockPin | kPS2KbdDataPin;
}
uint8_t HandlePS2Keyboard(uint8_t *code, uint8_t *keyup, uint8_t *extended) 
{
        static uint8_t ps2lastcode = 0;
        uint16_t ps2frame = 0;
        uint8_t ps2scancode;
        uint8_t tmpcode = 0;
        *code = *keyup = *extended = 0;
        PORTC ^= 1<<5;
        ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
        PORTC ^= 1<<5;
        ps2scancode = tmpcode = (ps2frame >> 1) & 0xFF;
        if(ps2scancode == 0xF0) {
                // Keyup, poll in the scancode of the key that has been released
                ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                ps2scancode = ps2frame >> 1;
                // Ignore keyup capslock if we're turning it on
                if(((ps2scancode) == 0x58) && (ps2ledstate & kPS2LEDCaps)) {
                } else {
                        *code = ps2scancode;
                        *keyup = 1;
                        *extended = 0;
                }
        } else if(ps2scancode == 0xE0) {
                // two byte scan code
                ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                ps2scancode = ps2frame >> 1;
                if(ps2scancode == 0xF0) {
                        ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                        ps2scancode = ps2frame >> 1;
                        if(ps2scancode == 0x7C) {
                                // prntscrn
                                ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                                ps2scancode = ps2frame >> 1;
                                // should be E0
                                ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                                ps2scancode = ps2frame >> 1;
                                // should be F0
                                ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                                ps2scancode = ps2frame >> 1;
                                // should be 12
                                return 0;
                        }
                        *code = ps2scancode;
                        *keyup = 1;
                        *extended = 1;
                } else if(ps2scancode == 0x12) {
                        // prntscrn
                        ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                        ps2scancode = ps2frame >> 1;
                        // should be E0
                        ps2frame = PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
                        ps2scancode = ps2frame >> 1;
                        // should be 7C
                        return 0;
                } else {
                        *code = ps2scancode;
                        *keyup = 0;
                        *extended = 1;
                }
        } else {
                if(ps2scancode == ps2lastcode) {
                        return 0;
                }
                // If it's capslock, turn on the capslock led
                if((ps2scancode) == 0x58) {
                        if(ps2ledstate & kPS2LEDCaps) {
                                ps2ledstate &= ~kPS2LEDCaps;
                        } else {
                                ps2ledstate |= kPS2LEDCaps;
                        }
                        SetPS2LEDState(ps2ledstate);
                }
                *code = ps2scancode;
                *keyup = 0;
                *extended = 0;
        }
        ps2lastcode = tmpcode;
        return ps2scancode;
}
void SetPS2LEDState(uint8_t cmd)
{
        delayMicroseconds(100);
        SendPS2KbdCommand(0xED, kPS2KbdClockPin, kPS2KbdDataPin);
        // wait for ack 0xFA
        PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
        delayMicroseconds(100);
        SendPS2KbdCommand(cmd, kPS2KbdClockPin, kPS2KbdDataPin);
        PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
}
void SetPS2Repeat(void)
{
        delayMicroseconds(100);
        SendPS2KbdCommand(0xF8, kPS2KbdClockPin, kPS2KbdDataPin);
        // wait for ack 0xFA
        PollPS2KbdFrame(kPS2KbdClockPin, kPS2KbdDataPin, 0);
        kPS2KbdModePort |= kPS2KbdClockPin;
        kPS2KbdOutPort &= kPS2KbdClockPin;
}
void SendPS2KbdCommand(uint8_t cmd, uint8_t clock, uint8_t data)
{
        uint8_t bits;
        uint8_t parity = 0;
        uint32_t mstimeout;
        kPS2KbdModePort |= clock;
        kPS2KbdOutPort &= clock;
        // pull clock low for >=100us
        delayMicroseconds(150);
        // Bring data line low
        kPS2KbdModePort |= data;
        kPS2KbdOutPort &= ~data;
        
        // Release clock
        kPS2KbdModePort &= ~clock;
        kPS2KbdInPort |= clock;
        // Might need to wait for the clock to go high after
        // releasing it...
        while((kPS2KbdInPort & clock) == 0);
        for(bits = 0; bits < 8; bits++) {
                // Wait for device to bring clock low
                mstimeout = millis();
                while(kPS2KbdInPort & clock) {
                        if(millis() - mstimeout > 100) return;  // no device?
                };
                if(cmd & (1<<bits)) {
                        kPS2KbdOutPort |= data;
                        parity++;
                } else {
                        kPS2KbdOutPort &= ~data;
                }
                // wait out the remainder of the low clock
                while((kPS2KbdInPort & clock) == 0);
        }
        // Data has been sent, now send parity
        while(kPS2KbdInPort & clock);
        if((parity & 0x01) == 0) {
                kPS2KbdOutPort |= data;
        } else {
                kPS2KbdOutPort &= ~data;
        }
        while((kPS2KbdInPort & clock) == 0);
        while(kPS2KbdInPort & clock);
        // Stop bit, release data line
        kPS2KbdModePort &= ~data;
        kPS2KbdOutPort |= data;
        while((kPS2KbdInPort & data) == 0);
        while(kPS2KbdInPort & data);
        while((kPS2KbdInPort & data) == 0);
}
uint16_t PollPS2KbdFrame(uint8_t clock, uint8_t data, uint32_t polltimeout)
{
        uint16_t ps2frame = 0;
        uint8_t bits;
        uint8_t parity = 0;
        while(kPS2KbdInPort & clock);
        for(bits = 0; bits < 11; bits++) {
                // device to host should be read on falling edge
                if(kPS2KbdInPort & data) ps2frame |= 1 << bits;
                while((kPS2KbdInPort & clock) == 0);
                if(bits == 10) break;
                while(kPS2KbdInPort & clock);
        }
        if(ps2frame & 1) {
                return kPS2ErrorCode;
        }
        if(!(ps2frame & (1<<10))) {
                return kPS2ErrorCode;
        }
        // Enforce parity
        for(bits = 1; bits < 9; bits++) {
                if(ps2frame & (1<<bits)) parity++;
        }
        if((ps2frame & (1<<9)) && ((parity & 0x01) == 0)) return ps2frame;
        if(!(ps2frame & (1<<9)) && ((parity & 0x01) == 1)) return ps2frame;
        return kPS2ErrorCode;
}
// MOUSE
void InitPS2Mouse(void)
{
  pinMode(kPS2MouseClockArduinoPin, INPUT);
  digitalWrite(kPS2MouseClockArduinoPin, HIGH);
  pinMode(kPS2MouseDataArduinoPin, INPUT);
  digitalWrite(kPS2MouseDataArduinoPin, HIGH);
}
void SetupPS2Mouse(void)
{
        uint16_t ps2frame = 0;
        uint8_t ps2scancode;
        uint8_t resetresult;
        uint8_t mouseid;
        uint8_t result;
        _delay_us(100);
        Serial.println("Initializing mouse");
        Serial.print("Sending mouse reset: ");
        // reset
        SendPS2MouseCommand(0xFF, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        result = ps2frame >> 1;
        
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        resetresult = ps2frame >> 1;
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        mouseid = ps2frame >> 1;
        Serial.print(result);
        Serial.print(" ");
        Serial.print(resetresult);
        Serial.print(" ");
        Serial.println(mouseid);
        Serial.print("Setting sample rate: ");
        // Set sample rate
        SendPS2MouseCommand(0xF3, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        result = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        SendPS2MouseCommand(0x64, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        ps2scancode = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        Serial.print(result, HEX);
        Serial.print(" ");
        Serial.println(ps2scancode, HEX);
        Serial.print("reading device type: ");
        // Read device type
        SendPS2MouseCommand(0xF2, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        result = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        ps2scancode = ps2frame >> 1;
        // ps2scancode should be mouseid
        Serial.print(result, HEX);
        Serial.print(" ");
        Serial.println(ps2scancode, HEX);
        Serial.print("Setting resolution: ");
        // Set resolution
        SendPS2MouseCommand(0xE8, kPS2MouseClockPin, kPS2MouseDataPin);
        while(kPS2MouseInPort & kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        result = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        SendPS2MouseCommand(0x04, kPS2MouseClockPin, kPS2MouseDataPin);
        while(kPS2MouseInPort & kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        ps2scancode = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        Serial.print(result);
        Serial.print(" ");
        Serial.println(ps2scancode, HEX);
        Serial.print("Setting scaling: ");
        // Set scaling
        SendPS2MouseCommand(0xE6, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        ps2scancode = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        Serial.println(ps2scancode, HEX);
        Serial.print("Enabling: ");
        // Enable
        SendPS2MouseCommand(0xF4, kPS2MouseClockPin, kPS2MouseDataPin);
        ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
        ps2scancode = ps2frame >> 1;
        // ps2scancode should be 0xFA for ack
        Serial.println(ps2scancode, HEX);
}
int HandlePS2Mouse(uint8_t *status, uint8_t *xmove, uint8_t *ymove)
{
        uint16_t ps2frame = 0;
        while(kPS2MouseInPort & kPS2MouseClockPin);
        ps2frame = 0;
        do {
                ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
                if(ps2frame == kPS2ErrorCode) {
                        //Serial.println("Failure on status");
                        return -1;
                }
        }while(ps2frame == kPS2RetryCode);
        *status = (ps2frame >> 1) & 0xFF;
        // self-test passed, a mouse was probably just attached
        if(*status == 0xAA) {
                // Read mouse id
                ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
                
                Serial.println("Re-setup mouse");
                //SetupPS2Mouse();
                return -1;
        }
        while(kPS2MouseInPort & kPS2MouseClockPin);
        ps2frame = 0;
        do {
                ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
                if(ps2frame == kPS2ErrorCode) {
                        Serial.println("Failure on xmove");
                        return -1;
                }
        }while(ps2frame == kPS2RetryCode);
        *xmove = (ps2frame >> 1) & 0xFF;
        while(kPS2MouseInPort & kPS2MouseClockPin);
        ps2frame = 0;
        do {
                ps2frame = PollPS2MouseFrame(kPS2MouseClockPin, kPS2MouseDataPin, 0);
                if(ps2frame == kPS2ErrorCode) {
                        Serial.println("Failure on ymove");
                        return -1;
                }
        }while(ps2frame == kPS2RetryCode);
        *ymove = (ps2frame >> 1) & 0xFF;
        if(!(*status & 0x08)) {
                //Serial.print("Status is missing bit 0x08: ");
                Serial.println(*status);
                return -1;
        }
#if 0 
        Serial.print("PS2: ");
        Serial.print(*status);
        Serial.print(*xmove);
        Serial.println(*ymove);
#endif
        return 0;
}
void SendPS2MouseCommand(uint8_t cmd, uint8_t clock, uint8_t data)
{
        uint8_t bits;
        uint8_t parity = 0;
        uint32_t mstimeout;
        kPS2MouseModePort |= clock;
        kPS2MouseOutPort &= ~clock;
        
        // pull clock low for >=100us
        _delay_us(150);
        // Bring data line low
        kPS2MouseModePort |= data;
        kPS2MouseOutPort &= ~data;
                
        // Release clock
        kPS2MouseModePort &= ~clock;
        kPS2MouseOutPort |= clock;
        // Might need to wait for the clock to go high after
        // releasing it...
        while((kPS2MouseInPort & clock) == 0);
        for(bits = 0; bits < 8; bits++) {
                // Wait for device to bring clock low
                mstimeout = millis();
                while(kPS2MouseInPort & clock) {
                        if(millis() - mstimeout > 100) return;  // no device?
                };
                if(cmd & (1<<bits)) {
                        kPS2MouseOutPort |= data;
                        parity++;
                } else {
                        kPS2MouseOutPort &= ~data;
                }
                // wait out the remainder of the low clock
                while((kPS2MouseInPort & clock) == 0);
        }
        // Data has been sent, now send parity
        while(kPS2MouseInPort & clock);
        if((parity & 0x01) == 0) {
                kPS2MouseOutPort |= data;
        } else {
                kPS2MouseOutPort &= ~data;
        }
        while((kPS2MouseInPort & clock) == 0);
        while(kPS2MouseInPort & clock);
        // Stop bit, release data line
        kPS2MouseModePort &= ~data;
        kPS2MouseOutPort |= data;
        while((kPS2MouseInPort & data) == 0);
        while(kPS2MouseInPort & data);
        while((kPS2MouseInPort & data) == 0);
}
uint16_t PollPS2MouseFrame(uint8_t clock, uint8_t data, uint32_t polltimeout)
{
        uint16_t ps2frame = 0;
        uint8_t bits;
        uint8_t parity = 0;
        //while(kPS2MouseInPort & clock);
        if(!wait_lo(&kPS2MouseInPort, clock, 50)) return kPS2ErrorCode;
        for(bits = 0; bits < 11; bits++) {
                // device to host should be read on falling edge
                if(kPS2MouseInPort & data) ps2frame |= 1 << bits;
                //while((kPS2MouseInPort & clock) == 0);
                if(!wait_hi(&kPS2MouseInPort, clock, 50)) return kPS2ErrorCode;
                if(bits == 10) break;
                //while(kPS2MouseInPort & clock);
                if(!wait_lo(&kPS2MouseInPort, clock, 50)) return kPS2ErrorCode;
        }
        if(ps2frame & 1) {
                //Serial.print("Start bit failure on ");
                //Serial.print(ps2frame >> 8, HEX);
                //Serial.println(ps2frame, HEX);
                return kPS2ErrorCode;
        }
        if(!(ps2frame & (1<<10))) {
                //Serial.print("Stop bit failure on ");
                //Serial.print(ps2frame >> 8, HEX);
                //Serial.println(ps2frame, HEX);
                return kPS2ErrorCode;
        }
        // Enforce parity
        for(bits = 1; bits < 9; bits++) {
                if(ps2frame & (1<<bits)) parity++;
        }
        if((ps2frame & (1<<9)) && ((parity & 0x01) == 0)) return ps2frame;
        if(!(ps2frame & (1<<9)) && ((parity & 0x01) == 1)) return ps2frame;
        //Serial.print("Parity failure on ");
        //Serial.print(ps2frame >> 8, HEX);
        //Serial.println(ps2frame, HEX);
        return kPS2ErrorCode;
}
