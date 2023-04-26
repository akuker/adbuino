
add_library(usb
    char2usbkeycode.cpp
    usbkbdparser.cpp
    usbmouseparser.cpp
    )

    include_directories(../../misc/include)
    target_link_libraries(usb QuokkADB)
    target_include_directories(usb PUBLIC ../include)