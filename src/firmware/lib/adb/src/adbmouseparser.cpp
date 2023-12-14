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
}

bool ADBMouseRptParser::MouseReady()
{
    return MouseChanged();
}

uint16_t ADBMouseRptParser::GetAdbRegister0()
{
    static bool button_left_last;
    static bool button_right_last;
    bool button_left = button_left_last;
    bool button_right = button_right_last;
    uint16_t reg_value = 0;;
    MOUSE_CLICK* click = nullptr;

    if (!m_click_events.isEmpty())
    {
        click = m_click_events.dequeue();
        button_left = click->bmLeftButton;
        button_right = click->bmRightButton;
    }

    // Bit 15 = Left Button Status; 0=down
    if (!button_left)
    {
        reg_value |= (1 << 15);
    }
    // Bit 7 = Right Button Status - introduced in System 8
    if (!button_right)
    {
        reg_value |= (1 << 7);
    }
    // Bits 14-8 = Y move Counts (Two's compliment. Negative = up, positive = down)
    reg_value |= AdjustMovement(m_coarse_y, m_fine_y) << 8;

    // Bits 6-0 = X move counts (Two's compliment. Negative = left, positive = right)
    reg_value |= AdjustMovement(m_coarse_x, m_fine_x) << 0;

    if (click != nullptr)
    {
        button_left_last = click->bmLeftButton;
        button_right_last = click->bmRightButton;
        delete click;
    }
    m_processed = true;
    return reg_value;
}
