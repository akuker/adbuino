// Minimal implementation of Arduino's Serial class
#pragma once

#include <stdlib.h>

enum print_type {
    BIN,
    OCT,
    DEC,
    HEX, 
};

namespace rp2040_serial {
    class RPSerial 
    {
        public:
            size_t print(int number);
            size_t print(int number, print_type type);
            size_t print(const char* string);
            size_t println(int number);
            size_t println(int number, print_type type);
            size_t println(const char* string);
    };
    
    extern RPSerial Serial;
}

