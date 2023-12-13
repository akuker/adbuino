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
#include <limits.h>
extern bool global_debug;

ADBMouseRptParser::ADBMouseRptParser(ADBKbdRptParser &kbd_parser)
{
    m_keyboard = &kbd_parser;
    m_divisor = DEFAULT_SENSITIVITY_DIVISOR;
    m_acc_x = 0;
    m_acc_y = 0;
    m_x = 0;
    m_y = 0;
    m_mouse_event = nullptr;
}

bool ADBMouseRptParser::Accumulate(int8_t usb_move, int32_t &adb_move, int32_t &accumulator)
{
    bool ready = false;

    if (usb_move / m_divisor == 0)
    {
        // The accumulator keeps track of fine movement
        accumulator += usb_move;
        if (accumulator >= m_divisor || accumulator <= -m_divisor)
        {
            ready = true;
        }
    }
    else
    {
        // standard movement when the adjusted movement is non-zero
        adb_move += usb_move;
        ready = true;
    }
    return ready;
}

int8_t ADBMouseRptParser::AdjustMovement(int32_t &adb_move, int32_t &accumulator)
{
    // adb_move is currently the sum of all usb movement
    // this is where the sensitivity is adjusted
    adb_move /= m_divisor;

    // Adjust adb movement with accumulated fine movements
    int8_t increment = accumulator / m_divisor;
    adb_move += increment;

    // Limits of a 7 bit number
    if (adb_move > 63) adb_move = 63;
    if (adb_move < -64) adb_move = -64;

    return (int8_t) adb_move & 0x7F;
}
bool ADBMouseRptParser::MouseReady()
{
    bool ready = false;
    if (!m_mouse_events.isEmpty())
    {
        m_mouse_event = m_mouse_events.dequeue();
        bool ready_x = Accumulate(m_mouse_event->dX, m_x, m_acc_x);
        bool ready_y = Accumulate(m_mouse_event->dY, m_y, m_acc_y);
        if (ready_x || ready_y) ready = true;
    }
    return ready;
}

uint16_t ADBMouseRptParser::GetAdbRegister0()
{
    uint16_t reg_value = 0;
    MOUSEINFO* peek;
    MOUSEINFO* new_event;

    // As long as the there is no button changes sum all USB movements
    while (!m_mouse_events.isEmpty())
    {
        peek = m_mouse_events.peek();
        if (peek->bmLeftButton != m_mouse_event->bmLeftButton ||
            peek->bmMiddleButton != m_mouse_event->bmMiddleButton ||
            peek->bmRightButton != m_mouse_event->bmRightButton)
        {
            break;
        }
        new_event = m_mouse_events.dequeue();
        Accumulate(new_event->dX, m_x, m_acc_x);
        Accumulate(new_event->dY, m_y, m_acc_y);

        delete new_event;
    }

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
    reg_value |= AdjustMovement(m_y, m_acc_y) << 8;

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= AdjustMovement(m_x, m_acc_x) << 0;

    delete m_mouse_event;
    m_x = 0;
    m_y = 0;
    // Keep left over accumulator movement
    m_acc_x = m_acc_x % m_divisor;
    m_acc_y = m_acc_y % m_divisor;

    m_mouse_event = nullptr;

    return reg_value;
}
