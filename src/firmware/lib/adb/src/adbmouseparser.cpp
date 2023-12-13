//---------------------------------------------------------------------------
//
//	ADBuino & QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the  ADBuino and the QuokkaADB projects.
//
//  This file is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
//  any later version.
//
//  This file is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with file. If not, see <https://www.gnu.org/licenses/>.
//
//----------------------------------------------------------------------------


#include "adbmouseparser.h"
#include <platform_logmsg.h>
#include <adb_platform.h>

extern bool global_debug;

ADBMouseRptParser::ADBMouseRptParser(ADBKbdRptParser &kbd_parser)
{
    m_keyboard = &kbd_parser;
    m_mouse_delay_ms_x = 0;
    m_mouse_delay_ms_y = 0;
    m_mouse_processing = false;
    m_divisor = DEFAULT_SENSITIVITY_DIVISOR;
}

uint32_t ADBMouseRptParser::EightBitToSevenBitSigned(int32_t value, int32_t compare)
{
    // This attempts to lower the USB mouse sensitivity
    int32_t adjusted_value = value / 7;
    // because the lowered mouse sensitivity may create 0 mouse movement
    // this attempts make sure the mouse moves in that case
    if (adjusted_value == 0 && value != 0)
    {
        // If both axis are zeroed because of the decreased sensitivity
        // This determines which one should be zero and which one should move
        if (abs(compare) / 3 > abs(value))
        {
            adjusted_value = 0;
        }
        else
        {
            adjusted_value = (value > 0) ? 1 : -1;
        }
    }
    return ((uint32_t)adjusted_value) & 0x7F;
}

bool ADBMouseRptParser::MouseReady()
{
    
    static bool button_change;
    if (m_mouse_processing)
    {
        bool ready = false;
        if (m_mouse_delay_x > 0) {
            if (--m_mouse_delay_x == 0)
            {
                m_x = m_mouse_event->dX > 0 ? 1 : -1;
                ready = true;
            }
            else m_x = 0;            
        }


        if (m_mouse_delay_y > 0) {
            if (--m_mouse_delay_y == 0)
            {
                m_y = m_mouse_event->dY > 0 ? 1 : -1;
                ready = true;
            }
            else m_y = 0;
        }
        
        while(!m_mouse_events.isEmpty())
        {
            delete m_mouse_events.dequeue();
        }
        return ready;
    }   
    else if (!m_mouse_events.isEmpty())
    {
        m_x = 0;
        m_y = 0;
        m_mouse_delay_x = 0;
        m_mouse_delay_y = 0;
        m_mouse_event = m_mouse_events.dequeue();
        uint32_t original_timestamp = m_mouse_event->timestamp;
        while(!m_mouse_events.isEmpty())
        {
            MOUSEINFO* peek = m_mouse_events.peek();
            // If there is no button change, skip all previous mouse movement
            // except for the last one which will be used for the preadjusted
            // ADB mouse movement
            if (m_mouse_event->bmLeftButton == peek->bmLeftButton &&
                m_mouse_event->bmMiddleButton == peek->bmMiddleButton &&
                m_mouse_event->bmRightButton == peek->bmRightButton
            )
            {
                MOUSEINFO *old_event = m_mouse_event;
                m_mouse_event = m_mouse_events.dequeue();

                delete old_event;

            }
            else
            {
                break;
            }
        }
        if (m_mouse_event->dX != 0 || m_mouse_event->dY != 0)
        {
            uint32_t delay;
            uint32_t time_factor = ((uint32_t)(millis() - original_timestamp)+9) / 10; // 10 ms is the ADB 
            if (m_mouse_event->dX / m_divisor == 0)
            {
                delay = m_divisor - abs(m_mouse_event->dX);
                m_mouse_delay_x = delay; 
            }
            else 
            {
                int32_t x;
                x = (int32_t) (abs(m_mouse_event->dX) / m_divisor);
                m_x = (int8_t) (m_mouse_event->dX > 0 ? x : -x);
                m_mouse_delay_x = 0;
            }

            if (m_mouse_event->dY / m_divisor  == 0 )
            {
                delay = m_divisor - abs(m_mouse_event->dY);

                m_mouse_delay_y = delay; 
            }
            else 
            {
                int32_t y;
                y = (int32_t) (abs(m_mouse_event->dY) / m_divisor);
                m_y = (int8_t) (m_mouse_event->dY > 0 ? y : -y);

                m_mouse_delay_y = 0;
            }
        }
        if (m_mouse_event->dX == 0)
        {
            m_x = 0;
        }
        if (m_mouse_event->dY == 0)
        {
            m_y = 0;
        }

        if (m_mouse_delay_x != 0 || m_mouse_delay_y != 0)
        {
            m_mouse_processing = true;
            return false;
        }
        return true;
    }
    return false;
}

uint16_t ADBMouseRptParser::GetAdbRegister0()
{
    uint16_t reg_value = 0;
    // this should never occur because the code should always check the queue before
    // executing this function
    // Never skip USB mouse input if there is a button change
 
    // Bit 15 = Left Button Status; 0=down
    if (!m_mouse_event->bmLeftButton)
    {
        reg_value |= (1 << 15);
    }
    // Bit 7 = Right Button Status - introduced in System 8
    if (!m_mouse_event->bmRightButton)
    {
        reg_value |= (1 << 7);
    }
    // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= (m_y & 0x7F) << 8;

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= (m_x & 0x7F) << 0;

    delete m_mouse_event;
    m_mouse_event = nullptr;

    if (m_mouse_delay_y == 0 && m_mouse_delay_x == 0)
    {
        m_mouse_processing = false;
    }

    return reg_value;
}
