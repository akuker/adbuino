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