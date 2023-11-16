//---------------------------------------------------------------------------
//
//	ADBuino keyboard and mouse adapter
//
//     Copyright (C) 2023 Rabbit Hole Computing LLC
//
//  This file is part of ADBuino.
//
//  This file is free software: you can redistribute it and/or modify it under 
//  the terms of the GNU General Public License as published by the Free 
//  Software Foundation, either version 3 of the License, or (at your option) 
// any later version.
//
//  This file is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
//  details.
//
//  You should have received a copy of the GNU General Public License along 
//  with this file. If not, see <https://www.gnu.org/licenses/>.
//
//  Portions of this code were originally released under a Modified BSD 
//  License. See LICENSE in the root of this repository for more info.
//
//----------------------------------------------------------------------------

// Logging system based on Arduino Serial library
#include "platform_logmsg.h"

platform::PLogmsg Logmsg;

namespace platform {
     
    size_t PLogmsg::print(int number) {
        return Serial.print(number, DEC);
    }

    size_t PLogmsg::print(int number, print_type type) {
        switch (type)
        {
        case fmtDEC :
            return Serial.print(number, DEC);
       case fmtBIN :
            return Serial.print(number, BIN);
        case fmtOCT :
            return Serial.print(number, OCT);
        case fmtHEX :
            return Serial.print(number, HEX);
        default:
            return 0;
            break;
        }
    }
    
    size_t PLogmsg::print(const char* string) {
        return Serial.print(string);

    }
    
    size_t PLogmsg::println(int number) {
        return Serial.println(number, DEC);
    }
    
    size_t PLogmsg::println(int number, print_type type) {
        switch (type)
        {
        case fmtDEC :
            return Serial.println(number, DEC);
       case fmtBIN :
            return Serial.println(number, BIN);
        case fmtOCT :
            return Serial.println(number, OCT);
        case fmtHEX :
            return Serial.println(number,HEX);
        default:
            return 0;
        }
    }
    
    size_t PLogmsg::println(const char* string) {
        return Serial.println(string);

    }
}
