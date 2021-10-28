    #include "wacom_messages.h"
    #include <string.h>
    #include <Arduino.h>
    #include <stdio.h>

    WacomRegister0::WacomRegister0(uint8_t *new_buffer){
        memcpy(m_data, new_buffer, sizeof(m_data));
        // The first two bits are alway set
        m_data[0] |= 0xC0;
    }
    
    void WacomRegister0::SetPosX(uint16_t pos){
        // Clear old data
        // Serial.print(" POS: ");
        // Serial.print(pos,HEX);
        // Serial.println(0xFF << 5, HEX);
        m_data[0] &= (0xFF & (0xFF << 5)); 
        m_data[0] |= (uint8_t)(0xFF & (pos >> (8+3)));
        // Serial.print("         0:");
        // Serial.print((uint8_t)(0xFF & (pos >> (8+3))), HEX);
        // Serial.print("Set 0: ");
        // Serial.print(" (pos >> (8+3) = ");
        // Serial.print( pos >> (3),HEX);
        // DumpData();
        m_data[1] &= 0x00;
        m_data[1] |= (uint8_t)(0xFF & (pos >> 3));
        // Serial.print("        1: ");
        // Serial.print((uint8_t)(0xFF & (pos >> 3)), HEX);
        // Serial.print("Set 1: ");
        // DumpData();
        m_data[2] &= (0xFF & (0xFF >> 3)); 
        m_data[2] |= (uint8_t)(0xFF & (pos << 5));
                // Serial.print("        2: ");
        // Serial.print((uint8_t)(0xFF & (pos << 5)), HEX);
        //  Serial.print("Set 2: ");
        //  DumpData();
    }


    void WacomRegister0::DumpData(){
        char data_str[10];
        for (int i = 0; i < sizeof(m_data); i++)
        {
          sprintf(data_str, " %02X", m_data[i]);
          Serial.print(data_str);
        }
        Serial.println("");
    }

    void WacomRegister0::SetPosY(uint16_t pos){
        m_data[2] &= (0xFF & (0xFF << 5)); 
        m_data[2] |= (uint8_t)(0xFF & (pos >> (8+3)));
        // Serial.print("Set 2: ");
        // Serial.print("pos: ");
        // Serial.print(pos,HEX);
        // Serial.print(" (pos >> (8+3) = ");
        // Serial.print( pos >> (8+3),HEX);
        // DumpData();
        m_data[3] &= 0x00;
        m_data[3] |= (uint8_t)(0xFF & (pos >> 3));
        // Serial.print("Set 3: ");
        // DumpData();
        m_data[4] &= (0xFF & (0xFF >> 3)); 
        m_data[4] |= (uint8_t)(0xFF & (pos << 5));
        // Serial.print("Set 4: ");
        // DumpData();




    }
    void WacomRegister0::SetPressure(uint16_t pressure){
        m_data[4] &= (0xFF & (0xFF << 5));
        m_data[4] |= (uint8_t)(pressure & 0x1F);
    }
    void WacomRegister0::SetIsTouching(bool touching){
        if(touching){
            m_data[0] |= 0x20;
        }else{
            m_data[0] &= 0xDF;
        }
    }
    uint16_t WacomRegister0::GetPosX(){
        uint16_t pos_x_a = ((((uint16_t)m_data[0])& 0x1F)<<8) + ((uint16_t)m_data[1]);
        pos_x_a = pos_x_a << 3;
        uint16_t pos_x_b = (uint16_t)(m_data[2] >> 5);
        return pos_x_a + pos_x_b;
    }

    uint16_t WacomRegister0::GetPressure(){
        uint16_t pressure = m_data[4] & 0x1F;
        return pressure;
    }

    uint16_t WacomRegister0::GetPosY(){

        // DumpData();
        uint16_t pos_y_a = ((uint16_t)m_data[2] & 0x1F) << (8 + 3);
        uint16_t pos_y_b = ((uint16_t)m_data[3]) << 3;
        uint16_t pos_y_c = ((uint16_t)m_data[4] & 0x00E0) >> 5;

        return pos_y_a + pos_y_b + pos_y_c;
    }

    bool WacomRegister0::GetIsTouching(){
        bool isTouching = (m_data[0] & 0x20) == 0x20;
        return isTouching;
    }

    void WacomRegister0::CopyDataTo(uint8_t *new_buffer){
        memcpy(new_buffer, m_data, sizeof(m_data));
    }

