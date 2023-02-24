
add_library(usb
    usbkbdparser.cpp
    usbmouseparser.cpp
    )

    include_directories(../../misc/include)
    target_link_libraries(usb QuokkADB)
    target_include_directories(usb PUBLIC ../include)