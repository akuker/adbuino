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

//---------------------------------------------------------------------------
//  This code emulates USB Host Shield 2.0 mouse and keyboard parsers
//  in "hidboot.h" for TinyUSB
//---------------------------------------------------------------------------
#pragma once

#include <stdint.h>
#include "tusb.h"
#include "platformkbdparser.h"
#include <scqueue.h>
using simple_circular_queue::SCQueue;

// USB response 1ms, worst ADB response < 160ms, fastest human clicks per second < 30 or 1 per 33ms
// Queue length if using a machine USB clicker 160
// Queue length for humans ~ 5
// Using 10
#define MOUSE_CLICK_QUEUE_CAPACITY (10)

//----------------------------------------------------------------------------
// Mouse handler
//----------------------------------------------------------------------------

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

struct MOUSE_CLICK
{
        struct {
                uint8_t bmLeftButton : 1;
                uint8_t bmRightButton : 1;
                uint8_t bmMiddleButton : 1;
                uint8_t bmDummy : 5;
        };
};

class PlatformMouseParser {

        union {
                MOUSEINFO mouseInfo;
                uint8_t bInfo[sizeof (MOUSEINFO)];
        } prevState;

public:

        void Parse(const hid_mouse_report_t* report);
protected:

        virtual void OnMouseMove(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnLeftButtonUp(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnLeftButtonDown(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnRightButtonUp(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnRightButtonDown(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnMiddleButtonUp(MOUSEINFO *mi __attribute__((unused))) {
        };

        virtual void OnMiddleButtonDown(MOUSEINFO *mi __attribute__((unused))) {
        };

        int8_t AdjustMovement(int32_t coarse, int32_t fine);

        SCQueue<MOUSE_CLICK*, MOUSE_CLICK_QUEUE_CAPACITY> m_click_events;

        PlatformKbdParser* m_keyboard;

        bool m_processed = false;
        bool m_ready = false;
        int32_t m_coarse_x = 0;
        int32_t m_coarse_y = 0;
        int32_t m_fine_x = 0;
        int32_t m_fine_y = 0;
};
