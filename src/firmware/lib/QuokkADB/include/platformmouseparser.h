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

#define MOUSE_QUEUE_CAPACITY (20)


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


        SCQueue<MOUSEINFO*, MOUSE_QUEUE_CAPACITY> m_mouse_events; 

        PlatformKbdParser* m_keyboard;
};
