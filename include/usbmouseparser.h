/**
 * QuokkADB
 * Copyright (C) 2022 Rabbit Hole Computing LLC
 * This file is derived from adbuino:  https://github.com/akuker/adbuino
 * see main.cpp for license information
 * 
 * 26 September 2022 - Modifed by Rabbit Hole Computing LLC
 */

#pragma once
#include <stdlib.h>

struct MOUSEINFO {

        struct {
                uint8_t bmLeftButton : 1;
                uint8_t bmRightButton : 1;
                uint8_t bmMiddleButton : 1;
                uint8_t bmDummy : 5;
        };
        int8_t dX;
        int8_t dY;
};

class MouseRptParser 
{
public:
    bool MouseChanged();
    int32_t GetDeltaX();
    int32_t GetDeltaY();
    void ResetMouseMovement();
    uint16_t GetAdbRegister0();
    bool MouseButtonIsPressed();

    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    uint32_t EightBitToSevenBitSigned(int8_t value);


private:
    int8_t m_movedx = 0;
    int8_t m_movedy = 0;
    bool m_mouse_left_button_is_pressed = false;
    bool m_mouse_right_button_is_pressed = false;
    bool m_mouse_button_changed = false;
};
