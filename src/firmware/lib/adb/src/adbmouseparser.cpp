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

extern bool global_debug;

ADBMouseRptParser::ADBMouseRptParser(ADBKbdRptParser &kbd_parser)
{
    m_keyboard = &kbd_parser;
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

uint16_t ADBMouseRptParser::GetAdbRegister0()
{
    int32_t x = 0;
    int32_t y = 0;
    bool static button_change = false;
    MOUSEINFO* original_mouse_event =  m_mouse_events.dequeue();
    MOUSEINFO* peek;

    uint16_t reg_value = 0;
    // this should never occur because the code should always check the queue before
    // executing this function
    if (original_mouse_event == NULL)
    {
        Logmsg.println("Mouse event dequeue failed, queue empty. Releasing mouse buttons.");
        return 0x88;
    }

    x = original_mouse_event->dX;
    y = original_mouse_event->dY;

    // Never skip USB mouse input if there is a button change
    if (!button_change)
    {
        while(!m_mouse_events.isEmpty())
        {
            peek = m_mouse_events.peek();
            // If there is no button change, skip all previous mouse movement
            // except for the last one which will be used for the preadjusted
            // ADB mouse movement
            if (original_mouse_event->bmLeftButton == peek->bmLeftButton &&
                original_mouse_event->bmMiddleButton == peek->bmMiddleButton &&
                original_mouse_event->bmRightButton == peek->bmRightButton
            )
            {
                MOUSEINFO *new_event = m_mouse_events.dequeue();


                x = new_event->dX;
                y = new_event->dY;

                delete new_event;

            }
            else
            {
                button_change = true; 
                break;
            }

        }
    }
    else
    {
        if (!m_mouse_events.isEmpty())
        {
            peek = m_mouse_events.peek();
            if  (peek->bmLeftButton != original_mouse_event->bmLeftButton ||
                 peek->bmMiddleButton != original_mouse_event->bmMiddleButton ||
                 peek->bmRightButton != original_mouse_event->bmRightButton
                )
            {
                button_change = true;
            }
            else
            {
                button_change = false;
            }
        }
        else
        {
            button_change = false;
        }
    }

    // Bit 15 = Left Button Status; 0=down
    if (!original_mouse_event->bmLeftButton)
    {
        reg_value |= (1 << 15);
    }
    // Bit 7 = Right Button Status - introduced in System 8
    if (!original_mouse_event->bmRightButton)
    {
        reg_value |= (1 << 7);
    }
    // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= (EightBitToSevenBitSigned(y, x) << 8);

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= (EightBitToSevenBitSigned(x, y) << 0);

    delete original_mouse_event;
    return reg_value;
}
