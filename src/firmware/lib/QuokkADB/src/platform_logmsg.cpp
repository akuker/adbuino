//---------------------------------------------------------------------------
//
//	QuokkADB ADB keyboard and mouse adapter
//
//     Copyright (C) 2023 Rabbit Hole Computing LLC
//
//  This file is part of QuokkADB.
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
#include <Arduino.h>
#include "platform_logmsg.h"
#include "quokkadb_gpio.h"
#include "log_cache.h"

platform::PLogmsg Logmsg;

namespace platform {
     
    size_t PLogmsg::print(int number) {
        log_raw(number);
        return 0;
    }

    size_t PLogmsg::print(int number, print_type type) {
        switch (type)
        {
        case fmtDEC :
            log_raw(number);
            return 0;
//        case BIN :
//            return printf("%b", number);
        // case fmtOCT :
        //     return Serial1.print(number, OCT);
        case fmtHEX :
            log_raw((uint32_t)number);
            return 0;
        default:
            return 0;
            break;
        }
    }
    
    size_t PLogmsg::print(const char* string) {
        log_raw(string);
        return 0;
    }
    
    size_t PLogmsg::println(int number) {
        log_raw(number, "\n");
        return 0;
    }
    
    size_t PLogmsg::println(int number, print_type type) {
        switch (type)
        {
        case fmtDEC :
            log_raw(number, "\n");
            return 0;
//        case BIN :
//            return printf("%b\n", number);
        // case fmtOCT :
        //     return Serial1.print(number, OCT);
        case fmtHEX :
            log_raw((uint32_t) number, "\n");
            return 0;
        default:
            return 0;
        }
    }
    
    size_t PLogmsg::println(const char* string) {
        log_raw(string, "\n");
        return 0;
    }
}
