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

// Minimal implementation of Arudino's Serial class
#include "rp2040_serial.h"
#include "quokkadb_gpio.h"
#include <pico/printf.h>

namespace rp2040_serial {
 
    RPSerial Serial;
    size_t RPSerial::print(int number) {
        return print(number, DEC);
    }

    size_t RPSerial::print(int number, print_type type) {
        switch (type)
        {
        case DEC :
            return printf("%d", number);
//        case BIN :
//            return printf("%b", number);
        case OCT :
            return printf("%o", number);
        case HEX :
            return printf("%X", number);
        default:
            return 0;
            break;
        }
    }
    
    size_t RPSerial::print(const char* string) {
        return printf("%s", string);

    }
    
    size_t RPSerial::println(int number) {
        return println(number, DEC);
    }
    
    size_t RPSerial::println(int number, print_type type) {
        switch (type)
        {
        case DEC :
            return printf("%d\n", number);
//        case BIN :
//            return printf("%b\n", number);
        case OCT :
            return printf("%o\n", number);
        case HEX :
            return printf("%X\n", number);
        default:
            return 0;
        }
    }
    
    size_t RPSerial::println(const char* string) {
        return printf("%s\n", string);

    }
}

void putchar_(char c){
    uart_putc(UART_PORT, c);
}