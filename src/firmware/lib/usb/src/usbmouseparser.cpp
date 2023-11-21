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

#include <Arduino.h>
#include "usbmouseparser.h"
#include "usb_hid_keys.h"
#include <platform_logmsg.h>


extern bool global_debug;

bool MouseRptParser::MouseChanged()
{
    return hasQueuedMouseInfo();
}
// int32_t MouseRptParser::GetDeltaX()
// {
//     return m_movedx;
// }
// int32_t MouseRptParser::GetDeltaY()
// {
//     return m_movedy;
// }
// void MouseRptParser::ResetMouseMovement()
// {
//     m_movedy = 0;
//     m_movedx = 0;
//     m_mouse_button_changed = false;
// }
// bool MouseRptParser::MouseButtonIsPressed()
// {
//     return m_mouse_left_button_is_pressed || m_mouse_right_button_is_pressed;
// }


void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.print("dx=");
        Logmsg.print(mi->dX, fmtDEC);
        Logmsg.print(" dy=");
        Logmsg.println(mi->dY, fmtDEC);
    }
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("L Bttn Up");
    }
 };
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("L Bttn Dn");
    }
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("R Bttn Up");
    }
    switch (m_right_btn_mode)
    {
        case MouseRightBtnMode::ctrl_click :

            mi->bmLeftButton = false;
            #ifdef QUOKKADB
            sleep_ms(100);
            #else
            delay(100);
            #endif
            while(m_keyboard->PendingKeyboardEvent());
            m_keyboard->OnKeyUp(0, USB_KEY_LEFTCTRL);
            while(m_keyboard->PendingKeyboardEvent());

        break;
        case MouseRightBtnMode::right_click :
            mi->bmRightButton = false;
        break;
    }

};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("R Bttn Dn");
    }
    switch (m_right_btn_mode)
    {
        case MouseRightBtnMode::ctrl_click :
            while(m_keyboard->PendingKeyboardEvent());
            m_keyboard->OnKeyDown(0, USB_KEY_LEFTCTRL);
            while(m_keyboard->PendingKeyboardEvent());
            #ifdef QUOKKADB
            sleep_ms(200);
            #else
            delay(200);
            #endif
            mi->bmLeftButton = true;
        break;
        case MouseRightBtnMode::right_click :
            mi->bmRightButton = true;
        break;
    }
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("M Bttn Up");
    }

};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("M Bttn Dn");
    }
    switch (m_right_btn_mode)
    {
        case MouseRightBtnMode::ctrl_click :
            m_right_btn_mode = MouseRightBtnMode::right_click; 
        break;
        case MouseRightBtnMode::right_click :
            m_right_btn_mode = MouseRightBtnMode::ctrl_click;
        break;
        
    }
};
