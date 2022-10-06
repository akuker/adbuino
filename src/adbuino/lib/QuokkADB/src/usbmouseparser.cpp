//---------------------------------------------------------------------------
//
//	QuokkaADB ADB keyboard and mouse adapter
//
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of QuokkaADB.
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


#include "usbmouseparser.h"

extern bool global_debug;

bool MouseRptParser::MouseChanged()
{
    return ((m_movedx != 0) || (m_movedy != 0) || m_mouse_button_changed);
}
int32_t MouseRptParser::GetDeltaX()
{
    return m_movedx;
}
int32_t MouseRptParser::GetDeltaY()
{
    return m_movedy;
}
void MouseRptParser::ResetMouseMovement()
{
    m_movedy = 0;
    m_movedx = 0;
    m_mouse_button_changed = false;
}
bool MouseRptParser::MouseButtonIsPressed()
{
    return m_mouse_left_button_is_pressed || m_mouse_right_button_is_pressed;
}

uint32_t MouseRptParser::EightBitToSevenBitSigned(int8_t value)
{
    // Dividing the value by 2 has 2 benefits:
    //   - Easy way to convert from 8 bit to 7 bit number
    //   - It "softens" the mouse movement. Modern optical
    //     mice seem a little jumpy.
    return (uint32_t)((value/2) & 0x7F);
}

uint16_t MouseRptParser::GetAdbRegister0()
{
    uint16_t reg_value = 0;
    // Bit 15 = Left Button Status; 0=down
    if (!m_mouse_left_button_is_pressed)
    {
        reg_value |= (1 << 15);
    }

    if (!m_mouse_right_button_is_pressed)
    {
        reg_value |= (1 << 7);
    }
    
    // Bit 7 = Not used. Always 1
    reg_value |= (1 << 7);
    // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= (EightBitToSevenBitSigned(GetDeltaY()) << 8);

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= (EightBitToSevenBitSigned(GetDeltaX()) << 0);

    ResetMouseMovement();

    return reg_value;
}

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    m_movedy = mi->dY;
    m_movedx = mi->dX;
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi)
{
    m_mouse_left_button_is_pressed = false;
    m_mouse_button_changed = true;
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi)
{
    m_mouse_left_button_is_pressed = true;
    m_mouse_button_changed = true;
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi)
{
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi)
{
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi)
{
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi)
{
};
