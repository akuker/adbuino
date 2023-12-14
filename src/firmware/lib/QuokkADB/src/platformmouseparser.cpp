//----------------------------------------------------------------------------
//
//  QuokkADB ADB keyboard and mouse adapter
//     Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of QuokkADB.
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
//  with this file. If not, see <https://www.gnu.org/licenses/>.
//
//  A majority of this code were originally released under a 
//  General Public License version 2 (GPL2) License.
//  See LICENSE in the root of this repository for more info.
//
//---------------------------------------------------------------------------

#include "platformmouseparser.h"
#include <platform_logmsg.h>
#include <adb_platform.h>
#include "tusb.h"
#include <pico/mutex.h>
#include "flashsettings.h"

extern FlashSettings setting_storage;

int8_t PlatformMouseParser::AdjustMovement(int32_t coarse, int32_t fine)
{
    uint8_t sensitivity_divisor = setting_storage.settings()->sensitivity_divisor;
    // adb_move is currently the sum of all usb movement
    // this is where the sensitivity is adjusted
    int32_t adjusted = coarse / sensitivity_divisor;

    // Adjust adb movement with accumulated fine movements
    int8_t increment = fine / sensitivity_divisor;
    adjusted += increment;

    // Limits of a 7 bit number
    if (adjusted > 63) adjusted = 63;
    if (adjusted < -64) adjusted = -64;

    return (int8_t) adjusted & 0x7F;
}

void PlatformMouseParser::Parse(const hid_mouse_report_t *report){
    uint8_t sensitivity_divisor = setting_storage.settings()->sensitivity_divisor;
    MOUSEINFO mouse_info = {0};

    if (m_processed)
    {
        m_coarse_x = 0;
        m_coarse_y = 0;
        m_fine_x %= sensitivity_divisor;
        m_fine_y %= sensitivity_divisor;
        m_processed = false;
    }

    mouse_info.bmLeftButton   = !!(report->buttons & MOUSE_BUTTON_LEFT);
    mouse_info.bmRightButton  = !!(report->buttons & MOUSE_BUTTON_RIGHT);
    mouse_info.bmMiddleButton = !!(report->buttons & MOUSE_BUTTON_MIDDLE);
    mouse_info.dX = report->x;
    mouse_info.dY = report->y;

    m_coarse_x += mouse_info.dX;
    m_coarse_y += mouse_info.dY;

    if (mouse_info.dX / sensitivity_divisor == 0) m_fine_x += mouse_info.dX;
    if (mouse_info.dY / sensitivity_divisor == 0) m_fine_y += mouse_info.dY;

    MOUSE_CLICK* click = new MOUSE_CLICK;
    click->bmLeftButton = mouse_info.bmLeftButton;
    click->bmMiddleButton = mouse_info.bmMiddleButton;
    click->bmRightButton = mouse_info.bmRightButton;
    if (m_click_events.isEmpty())
    {
        m_click_events.enqueue(click);
    }
    else
    {
        MOUSE_CLICK* peek;
        peek = m_click_events.peek();
        if (peek->bmLeftButton != click->bmLeftButton ||
            peek->bmMiddleButton != click->bmMiddleButton ||
            peek->bmRightButton != click->bmRightButton
            )
        {
            m_click_events.enqueue(click);
        }
    }

    if(mouse_info.dX != 0 || mouse_info.dY != 0) {
        OnMouseMove(&mouse_info);
    }

    // change to mouse left button down
    if (!prevState.mouseInfo.bmLeftButton && mouse_info.bmLeftButton) {
        OnLeftButtonDown(&mouse_info);
    }

    // change to mouse left button up
    if (prevState.mouseInfo.bmLeftButton && !mouse_info.bmLeftButton) {
        OnLeftButtonUp(&mouse_info);
    }

    if (!prevState.mouseInfo.bmRightButton && mouse_info.bmRightButton) {
        OnRightButtonDown(&mouse_info);
    }

    if (prevState.mouseInfo.bmRightButton && !mouse_info.bmRightButton) {
        OnRightButtonUp(&mouse_info);
    }

    if (!prevState.mouseInfo.bmMiddleButton && mouse_info.bmMiddleButton) {
        OnMiddleButtonDown(&mouse_info);
    }
    if (!prevState.mouseInfo.bmMiddleButton && mouse_info.bmMiddleButton) {
        OnMiddleButtonUp(&mouse_info);
    }

    m_ready = true;
}