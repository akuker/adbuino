//----------------------------------------------------------------------------
//
//  ADBuino & QuokkADB ADB keyboard and mouse adapter
//	   Copyright (C) 2021-2022 akuker
//     Copyright (C) 2007 Peter H Anderson
//     Copyright (C) 2022 Rabbit Hole Computing LLC
//
//  This file is part of the ABDuino and the QuokkADB projects.
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
//----------------------------------------------------------------------------
#pragma once

#ifdef QUOKKADB
#include <stdlib.h>
#endif

#include "platformmouseparser.h"

enum class MouseRightBtnMode {
    ctrl_click,
    right_click
};


class MouseRptParser : public PlatformMouseParser
{
public:
    bool MouseChanged();
    int32_t GetDeltaX();
    int32_t GetDeltaY();
    void ResetMouseMovement();
    bool MouseButtonIsPressed();

protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    inline bool IsLeftButtonPressed() {return m_mouse_left_button_is_pressed;}
    inline bool IsRightButtonPressed(){return m_mouse_right_button_is_pressed;}

private:
    int8_t m_movedx = 0;
    int8_t m_movedy = 0;
    bool m_mouse_left_button_is_pressed = false;
    bool m_mouse_right_button_is_pressed = false;
    bool m_mouse_button_changed = false;
    #ifdef QUOKKADB
    MouseRightBtnMode m_right_btn_mode = MouseRightBtnMode::ctrl_click;
    #else
    MouseRightBtnMode m_right_btn_mode = MouseRightBtnMode::right_click;
    #endif

};
