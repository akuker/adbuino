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
#include <platform_logmsg.h>


extern bool global_debug;

bool MouseRptParser::MouseChanged()
{
    bool changed = m_ready || !m_click_events.isEmpty();
    m_ready = false;
    return changed;
}

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
    MOUSE_CLICK* click = new MOUSE_CLICK;
    click->bmLeftButton = true;
    if (!m_click_events.enqueue(click))
    {
        Logmsg.println("Warning! unable to enqueue Click Down");
    }
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("R Bttn Up");
    }
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi)
{
    if (global_debug)
    {
        Logmsg.println("R Bttn Dn");
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
};


void MouseRptParser::Reset(void)
{
    while(!m_click_events.isEmpty())
    {
        delete m_click_events.dequeue();
    }
}
