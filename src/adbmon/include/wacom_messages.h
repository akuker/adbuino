
#include <stdint.h>

#define CHECKIF(x)                                          \
    {                                                       \
        if (x)                                              \
        { /*Serial.print(#x); Serial.println(" passed!");*/ \
        }                                                   \
        else                                                \
        {                                                   \
            Serial.print(#x);                               \
            Serial.println(" failed!");                     \
        }                                                   \
    }

class WacomRegister0
{
public:
    void SetPosX(uint16_t pos);
    void SetPosY(uint16_t pos);
    void SetPressure(uint16_t pressure);
    void SetIsTouching(bool touching);
    uint16_t GetPosX();
    uint16_t GetPosY();
    uint16_t GetPressure();
    bool GetIsTouching();

    void CopyDataTo(uint8_t *new_buffer);
    WacomRegister0(uint8_t *new_buffer);

    void DumpData();
    static void SelfTest();

protected:
    uint8_t m_data[5];
};