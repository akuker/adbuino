#include "wacom_messages.h"
#include <string.h>
#include <Arduino.h>
#include <stdio.h>

WacomRegister0::WacomRegister0(uint8_t *new_buffer)
{
    memcpy(m_data, new_buffer, sizeof(m_data));
    // The first two bits are alway set
    m_data[0] |= 0xC0;
}

void WacomRegister0::SetPosX(uint16_t pos)
{
    m_data[0] &= (0xFF & (0xFF << 5));
    m_data[0] |= (uint8_t)(0xFF & (pos >> (8 + 3)));
    m_data[1] &= 0x00;
    m_data[1] |= (uint8_t)(0xFF & (pos >> 3));
    m_data[2] &= (0xFF & (0xFF >> 3));
    m_data[2] |= (uint8_t)(0xFF & (pos << 5));
}

void WacomRegister0::DumpData()
{
    char data_str[10];
    for (uint16_t i = 0; i < sizeof(m_data); i++)
    {
        sprintf(data_str, " %02X", m_data[i]);
        Serial.print(data_str);
    }
    Serial.println("");
}

void WacomRegister0::SetPosY(uint16_t pos)
{
    m_data[2] &= (0xFF & (0xFF << 5));
    m_data[2] |= (uint8_t)(0xFF & (pos >> (8 + 3)));
    m_data[3] &= 0x00;
    m_data[3] |= (uint8_t)(0xFF & (pos >> 3));
    m_data[4] &= (0xFF & (0xFF >> 3));
    m_data[4] |= (uint8_t)(0xFF & (pos << 5));
}
void WacomRegister0::SetPressure(uint16_t pressure)
{
    m_data[4] &= (0xFF & (0xFF << 5));
    m_data[4] |= (uint8_t)(pressure & 0x1F);
}
void WacomRegister0::SetIsTouching(bool touching)
{
    if (touching)
    {
        m_data[0] |= 0x20;
    }
    else
    {
        m_data[0] &= 0xDF;
    }
}
uint16_t WacomRegister0::GetPosX()
{
    uint16_t pos_x_a = ((((uint16_t)m_data[0]) & 0x1F) << 8) + ((uint16_t)m_data[1]);
    pos_x_a = pos_x_a << 3;
    uint16_t pos_x_b = (uint16_t)(m_data[2] >> 5);
    return pos_x_a + pos_x_b;
}

uint16_t WacomRegister0::GetPressure()
{
    uint16_t pressure = m_data[4] & 0x1F;
    return pressure;
}

uint16_t WacomRegister0::GetPosY()
{

    // DumpData();
    uint16_t pos_y_a = ((uint16_t)m_data[2] & 0x1F) << (8 + 3);
    uint16_t pos_y_b = ((uint16_t)m_data[3]) << 3;
    uint16_t pos_y_c = ((uint16_t)m_data[4] & 0x00E0) >> 5;

    return pos_y_a + pos_y_b + pos_y_c;
}

bool WacomRegister0::GetIsTouching()
{
    bool isTouching = (m_data[0] & 0x20) == 0x20;
    return isTouching;
}

void WacomRegister0::CopyDataTo(uint8_t *new_buffer)
{
    memcpy(new_buffer, m_data, sizeof(m_data));
}

void WacomRegister0::SelfTest()
{
    uint8_t data_buffer[5] = {0};
    Serial.println("Starting WacomRegister0 SelfTest");
    WacomRegister0 *wacom = new WacomRegister0(data_buffer);

    wacom->SetPosY(0xFFFF);
    wacom->SetPosX(1);
    CHECKIF(wacom->GetPosX() == 0x0001)
    wacom->SetPosX(0);
    CHECKIF(wacom->GetPosX() == 0x0000)
    wacom->SetPosX(0xFFFF);
    CHECKIF(wacom->GetPosX() == 0xFFFF)
    wacom->SetPosX(0x1000);
    CHECKIF(wacom->GetPosX() == 0x1000)
    wacom->SetPosX(0x8000);
    CHECKIF(wacom->GetPosX() == 0x8000)
    CHECKIF(wacom->GetPosY() == 0xFFFF)
    wacom->SetPosX(0xFFFF);

    wacom->SetPosY(1);
    CHECKIF(wacom->GetPosY() == 0x0001)
    wacom->SetPosY(0);
    CHECKIF(wacom->GetPosY() == 0x0000)
    wacom->SetPosY(0xFFFF);
    CHECKIF(wacom->GetPosY() == 0xFFFF)
    wacom->SetPosY(0x1000);
    CHECKIF(wacom->GetPosY() == 0x1000)
    wacom->SetPosY(0x8000);
    CHECKIF(wacom->GetPosY() == 0x8000)

    wacom->SetPosX(0x0);
    wacom->SetPosY(1);
    CHECKIF(wacom->GetPosY() == 0x0001)
    wacom->SetPosY(0);
    CHECKIF(wacom->GetPosY() == 0x0000)
    wacom->SetPosY(0xFFFF);
    CHECKIF(wacom->GetPosY() == 0xFFFF)
    wacom->SetPosY(0x1000);
    CHECKIF(wacom->GetPosY() == 0x1000)
    wacom->SetPosY(0x8000);
    CHECKIF(wacom->GetPosY() == 0x8000)

    for (uint32_t x = 0; x <= 0xFFFF; x += random(1, 5000))
    {
        wacom->SetPosX(x);
        for (uint32_t y = 0; y <= 0xFFFF; y += random(1, 5000))
        {
            wacom->SetPosY(y);

            for (uint32_t p = 0; p <= 0x1F; p += random(1, 200))
            {
                wacom->SetPressure(p);
                CHECKIF(wacom->GetPosY() == y)
                CHECKIF(wacom->GetPosX() == x)
                CHECKIF(wacom->GetPressure() == p)
            }
        }
    }

    wacom->SetIsTouching(true);
    CHECKIF(wacom->GetIsTouching() == true);

    wacom->SetIsTouching(false);
    CHECKIF(wacom->GetIsTouching() == false);

    wacom->SetIsTouching(true);
    CHECKIF(wacom->GetIsTouching() == true);

    Serial.println("Done Testing!");
}